from scipy import random, linalg
import numpy

matrix_size = 6

eigen_mean = 2
A = random.rand(matrix_size, matrix_size)
B = numpy.dot(A.transpose(), numpy.identity(matrix_size) * eigen_mean)
B = numpy.dot(B, A)

print("2 3 3 6 6")
print(str(B)[1:-1].replace('[', '')
            .replace('\n', ' ')
            .replace(']', '\n')
            .replace(',', ' '))
