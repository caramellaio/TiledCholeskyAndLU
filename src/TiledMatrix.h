#ifndef __TILED_MATRIX_H__
#define __TILED_MATRIX_H__
#include <stdio.h>

typedef struct TiledMatrix_TAG {
  float* data;
  int n_blk;
  int m_blk;
  int side_blk;
} TiledMatrix;

float * TiledMatrix_get_block(TiledMatrix* self,
                              int i_blk, int j_blk);

void TiledMatrix_set_at(TiledMatrix* self,
                        int i_blk, int j_blk,
                        int i, int j, float val);

float TiledMatrix_get_at(TiledMatrix* self,
                         int i_blk, int j_blk,
                         int i, int j);

float TiledMatrix_get_val_non_tiled(TiledMatrix* self,
                                    int i, int j);

void TiledMatrix_print(TiledMatrix* self,
                       FILE *output);
#endif
