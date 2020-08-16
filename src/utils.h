#ifndef __UTILS__H__
#define __UTILS__H__
#include "TiledMatrix.h"

int Utils_get_length_infos(FILE *file,
                           TiledMatrix* matrix,
                           int* n, int* m);

int Utils_read_matrix(FILE *file,
                      TiledMatrix* matrix,
                      int n, int m);

#endif /* __UTILS__H__ */
