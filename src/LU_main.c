#include <stdio.h>
#include <stdlib.h>
#include "TiledMatrix.h"
#include "utils.h"
#include "tiledLU.h"

int main(int argc, char **argv)
{
  FILE *f;
  TiledMatrix tiled;
  int n, m;
  int rv;

  if (2 != argc) {
    printf("Error: expected 1 parameter, %d given\n", argc - 1);
    rv = 1;
    goto main_end;
  }

  f = fopen(argv[1], "r");

  if (NULL == f) {
    printf("Error opening file %s\n", argv[1]);
    goto main_end;
  }
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

#if 0
  printf("Printing input matrix:\n");
  TiledMatrix_print(&tiled, stdout, PRINT_ALL, NON_UNIT);
#endif
  fclose(f);

  printf("Calling tiled LU decomposition...\n");

  TiledLU_decompose(&tiled, NULL);

  printf("Tiled LU decomposition completed.\n");

#if 1
  printf("Printing resulting matrices: \n");
  printf("Printing L: \n");
  TiledMatrix_print(&tiled, stdout, PRINT_TRIANG_LOWER, UNIT);
  printf("Printing U: \n");
  TiledMatrix_print(&tiled, stdout, PRINT_TRIANG_UPPER, NON_UNIT);
#endif
  free(tiled.data);

#if 0
  f = fopen(argv[1], "r");
  rv = Utils_get_length_infos(f, &tiled, &n, &m);
  tiled.side_blk = n;
  tiled.n_blk = 1;
  tiled.m_blk = 1;
  rv = Utils_read_matrix(f, &tiled, n, m);

  printf("Calling NON tiled LU decomposition.\n");
  TiledLU_non_tiled_decompose(tiled.data, tiled.side_blk);
  printf("Printing result: \n");
  printf("Printing L:\n");
  TiledMatrix_print(&tiled, stdout, PRINT_TRIANG_LOWER, UNIT);
  printf("Printing U:\n");
  TiledMatrix_print(&tiled, stdout, PRINT_TRIANG_UPPER, NON_UNIT);
#endif
  main_end:
  return rv;
}
