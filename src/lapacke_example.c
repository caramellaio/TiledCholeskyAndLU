#include <stdio.h>
#include <lapacke.h>
#include "utils.h"
#include "TiledMatrix.h"

int main(int argc, char **argv)
{
  FILE *f;
  TiledMatrix tiled;
  int n, m;
  int rv;

  f = fopen("example_input.txt", "r");
  rv = Utils_get_length_infos(f, &tiled, &n, &m);

  if (0 != rv) {
    printf("Error reading matrix infos!\n");
    goto main_end;
  }

  printf("Matrix info: \nside_size: %d\n block_n: %d\n block_m:%d\n",
         tiled.side_blk, tiled.n_blk, tiled.m_blk);
  tiled.data = (double *)malloc(sizeof(double) * n * m);

  rv = Utils_read_matrix(f, &tiled, n, m);

  if (0 != rv) {
    printf("Error reading matrix data!\n");
    goto main_end;
  }

  TiledMatrix_print(&tiled, stdout);

  fclose(f);
  free(tiled.data);

  main_end:
  return rv;
}
