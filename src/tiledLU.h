#ifndef __TILED_LU_H__
#define __TILED_LU_H__

#include "TiledMatrix.h"

void TiledLU_decompose(TiledMatrix *tiled, TiledMatrix* tiled_pm);

void TiledLU_non_tiled_decompose(double *matrix, int size);
#endif /* __TILED_LU_H__ */
