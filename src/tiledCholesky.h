#ifndef __TILED_CHOLESKY_H__
#define __TILED_CHOLESKY_H__
#include "TiledMatrix.h"

void TiledCholesky_decompose(TiledMatrix* tiled);

void TiledCholesky_non_tiled_decompose(double* matrix, int size);
#endif /* __TILED_CHOLESKY_H__ */
