#include "TiledMatrix.h"
#include <assert.h>

#define GET_N(self) \
  (self->n_blk * self->side_blk)
#define GET_M(self) \
  (self->m_blk * self->side_blk)

/* NOTE: row major */
#define GET_ADDR(self, x_blk, y_blk, x, y) \
  (TiledMatrix_get_block(self, x_blk, y_blk) + y * self->side_blk + x)

/*
  BLOCKS STRUCTURE:

  block(0, 0) block(1, 0) ... block(n - 2, 0) block(n - 1, 0)
  block(0, 1) block(1, 1) ... block(n - 2, 1) block(n - 1, 1)
  ..........................................................
  ..........................................................
  ..........................................................
  block(0, m - 2) block(1, m - 2) ... block(n - 2, m - 2) block(n - 1, m - 2)
  block(0, m - 1) block(1, m - 1) ... block(n - 2, m - 1) block(n - 1, m - 1)

  Note: blocks on the same x are contiguous e.g. block(2, 3) is near block(3, 3)
*/
double *TiledMatrix_get_block(const TiledMatrix *self,
                              int x_blk, int y_blk)
{
  /* a column is the length of n rows */
  int add_y, add_x;

  add_y = y_blk * self->n_blk * self->side_blk * self->side_blk;
  add_x = x_blk * self->side_blk * self->side_blk;

  return self->data + add_x + add_y;
}


void TiledMatrix_set_at(TiledMatrix* self,
                        int x_blk, int y_blk,
                        int x, int y, double val)
{
  double *val_pointer = GET_ADDR(self, x_blk, y_blk, x, y);

  *val_pointer = val;
}

double TiledMatrix_get_at(const TiledMatrix* self,
                          int x_blk, int y_blk,
                          int x, int y)
{
  return *(GET_ADDR(self, x_blk, y_blk, x, y));
}

double TiledMatrix_get_val_non_tiled(const TiledMatrix* self,
                                     int x, int y)
{
  int blk_x, blk_y, pos_x, pos_y;

  blk_x = x / self->side_blk;
  pos_x = x % self->side_blk;

  blk_y = y / self->side_blk;
  pos_y = y % self->side_blk;

  return TiledMatrix_get_at(self, blk_x, blk_y, pos_x, pos_y);
}

void TiledMatrix_print(const TiledMatrix* self, FILE *output, int print_mode, int unit)
{
  int x, y;
  int n, m;

  n = GET_N(self);
  m = GET_M(self);

  PRINT_TRIANG_CHECK(print_mode);

  for (y = 0; y < m; y++) {
    for (x = 0; x < n; x++) {
      const char* format;
      double val;

      format = (x == m - 1) ? "%f\n" : "%f ";
      if (PRINT_ALL == print_mode ||
          (x >= y && PRINT_TRIANG_UPPER == print_mode) ||
          (y >= x && PRINT_TRIANG_LOWER == print_mode)) {
        /* unit matrices have diag = 1 */
        if (unit && y == x) {
          val = 1;
        }
        else {
          val = TiledMatrix_get_val_non_tiled(self, x, y);
        }
      }
      else {
        val = 0.;
      }

      fprintf(output, format, val);
    }
  }
}

int TiledMatrix_to_full(const TiledMatrix* self, double *full, int size)
{
  assert(0 < size);

  if (size != self->n_blk * self->side_blk) {
    printf("Incorrect size!!!\n");
    return 1;
  }

  /* TODO[AB]: Complete... */

  assert(0);

  return 0;
}
