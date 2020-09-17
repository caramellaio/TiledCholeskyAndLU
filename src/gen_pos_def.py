from scipy import random, linalg
import numpy
from argparse import ArgumentParser

def generate_pos_def_matrix(out_path, matrix_size, tile_size):
  eigen_mean = 2
  A = random.rand(matrix_size, matrix_size)
  B = numpy.dot(A.transpose(), numpy.identity(matrix_size) * eigen_mean)
  B = numpy.dot(B, A)

  with open(out_path, "w") as out_file:
    # first write the header
    tile_n = matrix_size / tile_size
    out_file.write("%d %d %d %d %d\n" % (tile_size, tile_n, tile_n, matrix_size, matrix_size))
    # write content
    out_file.write(str(B)[1:-1].replace('[', '')
                               .replace('\n', ' ')
                               .replace(']', '\n')
                               .replace(',', ' '))

  print("Matrix file %s correctly generated." % out_path)

def check_input(matrix_size, tile_size):
  if matrix_size % tile_size == 0 and matrix_size > 1 and tile_size > 0:
    print("Generating matrix with size NxN using tiles TxT where N=%d and T=%d\n" % (matrix_size, tile_size))
  else:
    print("Error: matrix size and tile size are not compatible N=%d and T=%d\n" % (matrix_size, tile_size))
    exit(1)

if __name__ == "__main__":
  parser = ArgumentParser()
  parser.add_argument("-n", "--matrix-size", type=int, required=True, help="Matrix size = NxN")
  parser.add_argument("-t", "--tile-size", type=int, required=True, help="tiled matrix = TXT")
  parser.add_argument("-o", "--out-file", type=str, required=True, help="file where the matrix will be generated")

  args = parser.parse_args()

  check_input(args.matrix_size, args.tile_size)

  generate_pos_def_matrix(args.out_file, args.matrix_size, args.tile_size)
