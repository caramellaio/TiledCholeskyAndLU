all:
	gcc -Wall utils.c TiledMatrix.c tiledCholesky.c cholesky_main.c -llapacke -llapack -lcblas -lm  -o TiledCholesky
	gcc -Wall utils.c TiledMatrix.c tiledCholesky.c cholesky_main.c -llapacke -llapack -lcblas -lm  -fopenmp -o TiledCholeskyOMP
	gcc -Wall utils.c TiledMatrix.c tiledLU.c LU_main.c -llapacke -llapack -lcblas -lm  -fopenmp -o TiledLUOMP
	gcc -Wall utils.c TiledMatrix.c tiledLU.c LU_main.c -llapacke -llapack -lcblas -lm  -o TiledLU
