#include "TiledMatrix.h"
#include <assert.h>

#define GET_N(self) \
  (self->n_blk * self->side_blk)
#define GET_M(self) \
  (self->m_blk * self->side_blk)

double * TiledMatrix_get_block(const TiledMatrix* self,
                              int i_blk, int j_blk)
{
  return self->data + i_blk * self->m_blk * self->side_blk * self->side_blk +
                    j_blk * self->side_blk * self->side_blk;
}

void TiledMatrix_set_at(TiledMatrix* self,
                        int i_blk, int j_blk,
                        int i, int j, double val)
{
  self->data[i_blk * self->m_blk * self->side_blk * self->side_blk +
             j_blk * self->side_blk * self->side_blk +
             i * self->side_blk + j] = val;
}

double TiledMatrix_get_at(const TiledMatrix* self,
                         int i_blk, int j_blk,
                         int i, int j)
{
  return self->data[i_blk * self->m_blk * self->side_blk * self->side_blk +
                    j_blk * self->side_blk * self->side_blk +
                    i * self->side_blk + j];
}

double TiledMatrix_get_val_non_tiled(const TiledMatrix* self,
                                    int i, int j)
{
  int blk_i, blk_j, pos_i, pos_j;

  if (1 == self->n_blk) {
    blk_i = 0;
    pos_i = i;
  }
  else {
    blk_i = i / self->n_blk;
    pos_i = i % self->n_blk;
  }


  if (1 == self->m_blk) {
    blk_j = 0;
    pos_j = j;
  }
  else {
    blk_j = j / self->m_blk;
    pos_j = j % self->m_blk;
  }

  return TiledMatrix_get_at(self, blk_i, blk_j, pos_i, pos_j);
}

void TiledMatrix_print(const TiledMatrix* self, FILE *output, int print_mode)
{
  int i, j;
  int n, m;

  n = GET_N(self);
  m = GET_M(self);

  PRINT_TRIANG_CHECK(print_mode);

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      const char* format;
      double val;

      format = (j == m - 1) ? "%f\n" : "%f ";
      if (PRINT_ALL == print_mode ||
          (i >= j && PRINT_TRIANG_LOWER == print_mode) ||
          (j >= i && PRINT_TRIANG_UPPER == print_mode)) {
        val = TiledMatrix_get_val_non_tiled(self, i, j);
      }
      else {
        val = 0.;
      }

      fprintf(output, format, val);
    }
  }
}

#if 0
void TiledMatrix_set_triangular_part(TiledMatrix* self, TriangPart p,
                                     int blk_i, int blk_j, double val)
{
  int i, j;
  double *matrix;

  matrix = TiledMatrix_get_block(self, blk_i, blk_j);

  for (i = 0; i < self->side_blk; i++) {
    for (j = i+1; j < self->side_blk; j++) {
      if (UPPER_TRIANG == p) {
        matrix[i * self->side_blk + j] = val;
      }
      else {
        assert(LOWER_TRIANG == p);
        matrix[j * self->side_blk + i] = val;
      }
    }
  }
}
#endif
