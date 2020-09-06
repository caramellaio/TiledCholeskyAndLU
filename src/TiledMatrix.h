#ifndef __TILED_MATRIX_H__
#define __TILED_MATRIX_H__
#include <stdio.h>
#include <assert.h>

#define PRINT_TRIANG_LOWER 0
#define PRINT_TRIANG_UPPER 1
#define PRINT_ALL 2
#define UNIT 1
#define NON_UNIT ! UNIT

#define PRINT_TRIANG_CHECK(p) \
  (assert(PRINT_TRIANG_LOWER <= p && PRINT_ALL >= p))

typedef struct TiledMatrix_TAG {
  double* data;
  int n_blk;
  int m_blk;
  int side_blk;
} TiledMatrix;

double * TiledMatrix_get_block(const TiledMatrix* self,
                              int i_blk, int j_blk);

void TiledMatrix_set_at(TiledMatrix* self,
                        int i_blk, int j_blk,
                        int i, int j, double val);

double TiledMatrix_get_at(const TiledMatrix* self,
                         int i_blk, int j_blk,
                         int i, int j);

double TiledMatrix_get_val_non_tiled(const TiledMatrix* self,
                                    int i, int j);

void TiledMatrix_print(const TiledMatrix* self,
                       FILE *output,
                       int print_mode,
                       int unit);

#if 0
void TiledMatrix_set_triangular_part(TiledMatrix* self, TriangPart p,
                                     int blk_i, int blk_j, double val);
#endif

#endif
