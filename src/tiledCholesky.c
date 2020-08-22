#include <assert.h>
#include <lapacke.h>
#include <cblas.h>
#include "tiledCholesky.h"

#define VERBOSE
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
    TiledMatrix_print(tiled, stdout, PRINT_TRIANG_LOWER);
#endif
    for (m = k + 1; m < t; m++) {
      double* A_m_k;

      A_m_k = TiledMatrix_get_block(tiled, m, k);


#if 0
      info = LAPACKE_dtrtrs(LAPACK_ROW_MAJOR, 'L', 'N', 'N',
                            tiled->side_blk, tiled->side_blk, A_k_k,
                            tiled->side_blk, A_m_k, tiled->side_blk);
#else

      cblas_dtrsm(CblasRowMajor, CblasRight, CblasLower, CblasTrans,
                  CblasNonUnit, tiled->side_blk, tiled->side_blk, 1.,
                  A_k_k, tiled->side_blk, A_m_k, tiled->side_blk);
#endif
    }

    for (n = k + 1; n < t; n++) {
      double* A_n_n;
      double* A_n_k;

      A_n_n = TiledMatrix_get_block(tiled, n, n);
      A_n_k = TiledMatrix_get_block(tiled, n, k);
      cblas_dsyrk(CblasRowMajor, CblasLower, CblasNoTrans,
                  tiled->side_blk, tiled->side_blk, 1.,
                  A_n_k, tiled->side_blk, 1., A_n_n, tiled->side_blk);

      for (m = n + 1; m < t; m++) {
        double* A_m_n, * A_m_k;

        A_m_n = TiledMatrix_get_block(tiled, m, n);
        A_m_k = TiledMatrix_get_block(tiled, m, k);

        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    tiled->side_blk, tiled->side_blk, tiled->side_blk,
                    1., A_m_k, tiled->side_blk, A_n_k, tiled->side_blk, 1.,
                    A_m_n, tiled->side_blk);
      }
    }

    (void)info;
  }
}

void TiledCholesky_non_tiled_decompose(double* matrix, int size)
{
  int info;

  info = LAPACKE_dpotrf(LAPACK_ROW_MAJOR, 'L', size, matrix, size);

  /* info must be 0 */
  assert(! info);
}
