from scipy import random, linalg
import numpy

matrix_size = 9

eigen_mean = 2
A = random.rand(matrix_size, matrix_size)
B = numpy.dot(A.transpose(), numpy.identity(matrix_size) * eigen_mean)
B = numpy.dot(B, A)

print("3 3 3 9 9")
print(str(B)[1:-1].replace('[', '')
            .replace('\n', ' ')
            .replace(']', '\n')
            .replace(',', ' '))
