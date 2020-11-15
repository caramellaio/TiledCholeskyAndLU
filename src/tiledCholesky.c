#include <assert.h>
#include <lapacke.h>
#include <cblas.h>
#include "tiledCholesky.h"

#if 0
#define VERBOSE
#endif

void TiledCholesky_decompose(TiledMatrix *tiled)
{
  int m_blk, n_blk;
  int k, m, n, t;

  m_blk = tiled->m_blk;
  n_blk = tiled->n_blk;

  /* expect nXn matrix */
  assert(n_blk == m_blk);
  t = n_blk;

  for (k = 0; k < t; k++) {
    double* A_k_k;
    int info;

    A_k_k = TiledMatrix_get_block(tiled, k, k);
    info = LAPACKE_dpotrf(LAPACK_ROW_MAJOR, 'L', tiled->side_blk, A_k_k, tiled->side_blk);
    //TiledMatrix_set_triangular_part(tiled, UPPER_TRIANG, k, k, 0.0);

#ifdef VERBOSE
    printf("Info val = %d\n", info);
    printf("Printing matrix after %d-th dpotrf:\n", k);
    TiledMatrix_print(tiled, stdout, PRINT_TRIANG_LOWER, NON_UNIT);
#endif

    #pragma omp parallel for
    for (m = k + 1; m < t; m++) {
      double* A_m_k;

      A_m_k = TiledMatrix_get_block(tiled, k, m);


#ifdef DEBUG
#ifdef _OPENMP
      printf("Thread %d is calling DTRSM\n", omp_get_thread_num());
#endif
#endif
      cblas_dtrsm(CblasRowMajor, CblasRight, CblasLower, CblasTrans,
                  CblasNonUnit, tiled->side_blk, tiled->side_blk, 1.,
                  A_k_k, tiled->side_blk, A_m_k, tiled->side_blk);
    }

    for (n = k + 1; n < t; n++) {
      double* A_n_n;
      double* A_n_k;

#ifdef DEBUG
#ifdef _OPENMP
      printf("Thread %d is calling DSYRK\n", omp_get_thread_num());
#endif
#endif
      A_n_n = TiledMatrix_get_block(tiled, n, n);
      A_n_k = TiledMatrix_get_block(tiled, k, n);
      cblas_dsyrk(CblasRowMajor, CblasLower, CblasNoTrans,
                  tiled->side_blk, tiled->side_blk, -1.,
                  A_n_k, tiled->side_blk, 1., A_n_n, tiled->side_blk);

      #pragma omp parallel for
      for (m = n + 1; m < t; m++) {
        double* A_m_n, * A_m_k;

        A_m_n = TiledMatrix_get_block(tiled, n, m);
        A_m_k = TiledMatrix_get_block(tiled, k, m);

#ifdef DEBUG
#ifdef _OPENMP
        printf("Thread %d is calling DGEMM\n", omp_get_thread_num());
#endif
#endif
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    tiled->side_blk, tiled->side_blk, tiled->side_blk,
                    -1., A_m_k, tiled->side_blk, A_n_k, tiled->side_blk, 1.,
                    A_m_n, tiled->side_blk);
      }
    }

    (void)info;
  }
}
