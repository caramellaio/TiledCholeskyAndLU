#ifndef __TILED_MATRIX_H__
#define __TILED_MATRIX_H__
#include <stdio.h>

typedef struct TiledMatrix_TAG {
  double* data;
  int n_blk;
  int m_blk;
  int side_blk;
} TiledMatrix;

typedef enum TriangPart_TAG {
  LOWER_TRIANG,
  UPPER_TRIANG
} TriangPart;

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
                       FILE *output);

void TiledMatrix_set_triangular_part(TiledMatrix* self, TriangPart p,
                                     int blk_i, int blk_j, double val);
#endif
