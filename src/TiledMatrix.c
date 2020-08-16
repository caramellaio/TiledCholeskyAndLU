#include "TiledMatrix.h"
#include <assert.h>

#define GET_N(self) \
  (self->n_blk * self->side_blk)
#define GET_M(self) \
  (self->m_blk * self->side_blk)

float * TiledMatrix_get_block(TiledMatrix* self,
                              int i_blk, int j_blk)
{
  return self->data + i_blk * self->m_blk * self->side_blk * self->side_blk +
                    j_blk * self->side_blk * self->side_blk;
}

void TiledMatrix_set_at(TiledMatrix* self,
                        int i_blk, int j_blk,
                        int i, int j, float val)
{
  self->data[i_blk * self->m_blk * self->side_blk * self->side_blk +
             j_blk * self->side_blk * self->side_blk +
             i * self->side_blk + j] = val;
}

float TiledMatrix_get_at(TiledMatrix* self,
                         int i_blk, int j_blk,
                         int i, int j)
{
  return self->data[i_blk * self->m_blk * self->side_blk * self->side_blk +
                    j_blk * self->side_blk * self->side_blk +
                    i * self->side_blk + j];
}

float TiledMatrix_get_val_non_tiled(TiledMatrix* self,
                                    int i, int j)
{
  int blk_i, blk_j, pos_i, pos_j;

  blk_i = i / self->n_blk;
  blk_j = j / self->m_blk;

  pos_i = i % self->n_blk;
  pos_j = j % self->m_blk;

  return TiledMatrix_get_at(self, blk_i, blk_j, pos_i, pos_j);
}

void TiledMatrix_print(TiledMatrix* self, FILE *output)
{
  int i, j;
  int n, m;

  n = GET_N(self);
  m = GET_M(self);

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      const char* format;
      float val;

      format = (j == m - 1) ? "%f\n" : "%f ";
      val = TiledMatrix_get_val_non_tiled(self, i, j);

      fprintf(output, format, val);
    }
  }
}
