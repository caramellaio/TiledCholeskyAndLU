#include <stdio.h>
#include <assert.h>
#include "utils.h"
#include "TiledMatrix.h"

int Utils_get_length_infos(FILE *file,
                           TiledMatrix* matrix,
                           int* n, int* m)
{
  int rv;

  rv = fscanf(file, "%d", &(matrix->side_blk));

  if (EOF == rv) goto get_length_exit;

  rv = fscanf(file, "%d", &(matrix->m_blk));

  if (EOF == rv) goto get_length_exit;

  rv = fscanf(file, "%d", &(matrix->n_blk));

  if (EOF == rv) goto get_length_exit;
  rv = fscanf(file, "%d", n);

  if (EOF == rv) goto get_length_exit;

  rv = fscanf(file, "%d", m);

  /* We assume that file does not end on this line */
  get_length_exit:

  /* 0 success, 1 failure */
  rv = EOF == rv;
  return rv;
}

/* TODO: document parameters */
int Utils_read_matrix(FILE *file,
                      TiledMatrix* matrix,
                      int n, int m)
{
  int rv;
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      float val;
      int i_blk, j_blk;
      int i_pos, j_pos;

      /* get i,j value */
      rv = fscanf(file, "%f", &val);
      if (EOF == rv) goto read_matrix_exit;

      /* calculate coordinates */
      i_blk = i / matrix->side_blk;
      i_pos = i % matrix->side_blk;
      j_blk = j / matrix->side_blk;
      j_pos = j % matrix->side_blk;

      /* TODO: here we assume row_maj */
      TiledMatrix_set_at(matrix, i_blk, j_blk, i_pos, j_pos, val);
      assert(val == TiledMatrix_get_at(matrix, i_blk, j_blk, i_pos, j_pos));
    }
  }

  rv = 0;

  read_matrix_exit:
  return rv;
}
