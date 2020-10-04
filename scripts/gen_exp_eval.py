from argparse import ArgumentParser
import gen_pos_def
import os
import threading

SHELL_SCRIPT_NAME = "exp_eval.sh"
DEF_OUT_DIR = "exp_eval"
BIN_FILES = ["TiledLU", "TiledLUOMP", "TiledCholesky", "TiledCholeskyOMP"]

def generate_matrices(start, delta, tile_size, num_exp, out_dir):
  for i in range(num_exp):
    matrix_size = start + i * delta
    out_path = os.path.join(out_dir, "%d_%d.txt" % (matrix_size, tile_size))

    print("Generating matrix with size: %d" % matrix_size)
    gen_pos_def_m_async(out_path, matrix_size, tile_size)

def gen_pos_def_m_async(out_path, matrix_size, tile_size):
  thr = threading.Thread(target=gen_pos_def.generate_pos_def_matrix,
                         args=(out_path, matrix_size, tile_size), kwargs={})
  thr.start()

def gen_shell_script(start, delta, tile_size, num_exp, out_dir, executables):
  script = os.path.join(out_dir, SHELL_SCRIPT_NAME)

  with open(script, "w") as script_w:
    for i in range(num_exp):
      matrix_size = start + i * delta

      j = 0
      for tool in executables:
        script_w.write("(time %s %d_%d.txt) 2> time_%d_%s.txt\n" %
                       (tool, matrix_size, tile_size, i, BIN_FILES[j]))
        j += 1


def check_params(args):
  rv = True

  if args.num_exp <= 0 or \
     args.tile_size <= 0 or \
     args.delta_size <= 0 or \
     args.start <= 0:
    print("Error: every integer parameter is required to be > 0\n")
    rv = False

  if os.path.exists(args.out_dir):
    if os.path.isdir(args.out_dir):
      print("Error: directory %s already exists.\n" % args.out_dir)
    else:
      print("Error: %s exists and it is a file.\n" % args.out_dir)

    rv = False
  else:
    print("Generating output folder %s\n" % args.out_dir)
    os.mkdir(args.out_dir)


  for tool in BIN_FILES:
    bin_path = os.path.join(args.bin_folder, tool)

    if not os.path.exists(bin_path):
      print("Error: file %s does not exist!\n" % bin_path)
      rv = False
    elif not os.path.isfile(bin_path):
      print("Error: %s is not a file!\n" % bin_path)
      rv = False

  if args.avoid_matrix_gen and args.avoid_script_gen:
    print("Error: avoid matrix gen and avoid script gen are both true\n")
  return rv

if __name__ == "__main__":
  parser = ArgumentParser()
  parser.add_argument("-n", "--num-exp", type=int, default=16, help="Number of matrix to check")
  parser.add_argument("-t", "--tile-size", type=int, default=5, help="tiled matrix = TXT")
  parser.add_argument("-d", "--delta-size", type=int, default=250, help="delta value for exp [def=300]")
  parser.add_argument("-s", "--start", type=int, default=3000, help="initial matrix size [def=5000]")
  parser.add_argument("-o", "--out-dir", type=str, default=DEF_OUT_DIR, help="output directory")
  parser.add_argument("-a", "--avoid-matrix-gen", type=bool, default=False, help="skip matrices generation")
  parser.add_argument("-k", "--avoid-script-gen", type=bool, default=False, help="skip script generation")
  parser.add_argument("-b", "--bin-folder", type=str, default= "../src", help="tools folder")


  args = parser.parse_args()

  if not check_params(args):
    print("Error: parameters not well-formed...\n")
    exit(1)

  if not args.avoid_matrix_gen:
    print("Starting matrices generation...\n")
    generate_matrices(args.start, args.delta_size, args.tile_size,
                      args.num_exp, args.out_dir)
    print("Matrices generation completed!\n")
  else:
    print("Matrices generation process skipped...\n")



  if not args.avoid_script_gen:
    print("Generating shell script...\n")

    executables = [os.path.join(args.bin_folder, tool) for tool in BIN_FILES]
    gen_shell_script(args.start, args.delta_size, args.tile_size,
                     args.num_exp, args.out_dir, executables)

    print("Shell script written in %s" % os.path.join(args.out_dir, SHELL_SCRIPT_NAME))
  else:
    print("Shell script generation process skipped...\n")

