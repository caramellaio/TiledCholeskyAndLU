#include <assert.h>
#include <lapacke.h>
#include <cblas.h>
#include "tiledLU.h"
#include "utils.h"

static void print_ipiv(int *ipiv, FILE* output, int size);

static void calc_k1_k2(int *ipiv, int* k1, int* k2, int size);

static void lu_decompose(double * matrix, int size);

#define VERBOSE
void TiledLU_decompose(TiledMatrix *tiled, TiledMatrix* tiled_pm)
{
  int *ipiv;
  int m_blk, n_blk;
  int k, i, j, t;

  m_blk = tiled->m_blk;
  n_blk = tiled->n_blk;

  /* expect nXn matrix */
  assert(n_blk == m_blk);
  t = n_blk;


  ipiv = (int*)malloc(sizeof(int) * tiled->side_blk);

  for (k = 0; k < t; k++) {
    double* A_k_k;
    int info, k1, k2;

    A_k_k = TiledMatrix_get_block(tiled, k, k);

#if 0
    /* LU decomposition of A[k, k] */
    info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, tiled->side_blk, tiled->side_blk,
                          A_k_k, tiled->side_blk, ipiv);

    if (0 > info) {
      printf("Error: failed dgetrf!!!\n");
      return;
    }

    print_ipiv(ipiv, stdout, tiled->side_blk);
    calc_k1_k2(ipiv, &k1, &k2, tiled->side_blk);

#endif
    lu_decompose(A_k_k, tiled->side_blk);

    #pragma omp parallel for shared(tiled, ipiv)
    for (i = k + 1; i < t; i++) {
      double* A_k_i;

      /* check order */
      A_k_i = TiledMatrix_get_block(tiled, i, k);


#if 0
      if (-1 != k1 && -1 != k2) {
        LAPACKE_dlaswp(LAPACK_ROW_MAJOR, tiled->side_blk, A_k_i, tiled->side_blk,
                       1, tiled->side_blk, ipiv, -1);
      }
      else {
        /* check correct input */
        assert(k2 == k1);
      }
#endif

      /* triangolar solve: case we do A[k][i] = L[k][k]^{-1} * A[k][i] */
      /* L is UNIT!!! */
      /* L[k][k] * X = A[k][i]*/
      cblas_dtrsm(CblasRowMajor, CblasLeft, CblasLower, CblasNoTrans,
                  CblasUnit, tiled->side_blk, tiled->side_blk, 1.,
                  A_k_k, tiled->side_blk, A_k_i, tiled->side_blk);
    }

    #pragma omp parallel for shared(tiled, ipiv)
    for (i = k + 1; i < t; i++) {
      double *A_i_k;

      A_i_k = TiledMatrix_get_block(tiled, k, i);

#if 0
      if (-1 != k1 && -1 != k2) {
        LAPACKE_dlaswp(LAPACK_ROW_MAJOR, tiled->side_blk, A_i_k, tiled->side_blk,
                       1, tiled->side_blk, ipiv, -1);
      }
      else {
        /* check correct input */
        assert(k2 == k1);
      }

#endif
      /* if I do it "COLUMN_MAJOR" it applies to columns??? */

      /* triangolar solve: case we do A[i][k] = A[i][k] * U[k][k]^{-1} */
      /* X * U[k][k] = A[i][k] => X = A[i][k] * U[k][k]^{-1} */
      /* re-check here. */
      cblas_dtrsm(CblasRowMajor, CblasRight, CblasUpper, CblasNoTrans,
                  CblasNonUnit, tiled->side_blk, tiled->side_blk, 1.,
                  A_k_k, tiled->side_blk, A_i_k, tiled->side_blk);
    }

    #pragma omp parallel for shared(tiled, ipiv)
    for (i = k +1; i < t; i++) {
      double *A_i_k;

      A_i_k = TiledMatrix_get_block(tiled, k, i);

      #pragma omp parallel for shared(tiled)
      for (j = k + 1; j < t; j++) {
        double *A_i_j, *A_k_j;

        A_i_j = TiledMatrix_get_block(tiled, j, i);
        A_k_j = TiledMatrix_get_block(tiled, j, k);

        /* 100% sure... */
        /* A[i][j] = A[i][j] - A[i][k] * A[k][j] */
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    tiled->side_blk, tiled->side_blk, tiled->side_blk,
                    -1., A_i_k, tiled->side_blk, A_k_j, tiled->side_blk, 1.,
                    A_i_j, tiled->side_blk);
      }
    }

    (void)info;
  }
}

void TiledLU_non_tiled_decompose(double *matrix, int size)
{
  int *ipiv;

  ipiv = (int*)malloc(sizeof(int)*size);
  (void)LAPACKE_dgetrf(LAPACK_ROW_MAJOR, size, size, matrix, size, ipiv);
  printf("Pivoting:\n");
  print_ipiv(ipiv, stdout, size);

  free(ipiv);
}

static void print_ipiv(int *ipiv, FILE* output, int size)
{
  int i;

  assert(0 < size);

  for (i = 0; i < size; i++) {
   char* format;
   format = (i == size -1) ? "%d\n" : "%d ";
   fprintf(output, format, ipiv[i]);
  }
}

static void calc_k1_k2(int *ipiv, int* k1, int* k2, int size)
{
  int i;

  *k2 = *k1 = -1;
  for (i = 0; i < size; i++) {
    if (ipiv[i] != i + 1) {
      /* get the first index */
      if (-1 == *k1) *k1 = i;

      /* last element */
      *k2 = i;
    }
  }
}

static void lu_decompose(double * matrix, int size)
{
  for(int j = 0; j < size; j++) {
    for(int i = 0; i < size; i++) {
      if(i<=j) {
        /* compute U part */

        //U[i][j]=A[i][j];
        double alpha;

        alpha = MATRIX_AT(matrix, size, j, i);

        for(int k=0; k < i; k++) {
          //U[i][j]-=L[i][k]*U[k][j];
          alpha -= MATRIX_AT(matrix, size, k, i) * MATRIX_AT(matrix, size, j, k);
        }

        MATRIX_AT(matrix, size, j, i) = alpha;
      }
      else {
        /* compute L part */
        double alpha;

        alpha = MATRIX_AT(matrix, size, j, i);
        //L[i][j]=A[i][j];

        for(int k = 0; k <= j - 1; k++) {
          //L[i][j]-=L[i][k]*U[k][j];
          alpha -= MATRIX_AT(matrix, size, k, i) * MATRIX_AT(matrix, size, j, k);
        }

        MATRIX_AT(matrix, size, j, i) = alpha;

        // L[i][j]/=U[j][j];
        MATRIX_AT(matrix, size, j, i) /= MATRIX_AT(matrix, size, j, j);
      }
    }
  }
}
