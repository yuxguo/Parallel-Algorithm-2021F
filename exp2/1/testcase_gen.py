import numpy as np;
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("--dir", type=str, required=True)
args = parser.parse_args()

def to_file(mats, dir):
    os.system("mkdir -p %s" % dir)
    A, L, U = mats
    with open(os.path.join(dir, "A.txt"), "w") as f:
        f.write("%d\n" % A.shape[0])
        for i in range(A.shape[0]):
            for j in range(A.shape[1]):
                f.write("%.6f\t" % A[i, j])
            f.write("\n")
    with open(os.path.join(dir, "result.txt"), "w") as f:
        f.write("L:\n")
        for i in range(L.shape[0]):
            for j in range(L.shape[1]):
                f.write("%.6f\t" % L[i, j])
            f.write("\n")
        f.write("U:\n")
        for i in range(U.shape[0]):
            for j in range(U.shape[1]):
                f.write("%.6f\t" % U[i, j])
            f.write("\n")

def matrix_gen(n):
    A = np.random.rand(n, n)
    L = np.tril(A, -1).copy() + np.eye(n)
    U = np.triu(A).copy()

    return L @ U, L, U

def main():
    shape_list = [16, 32, 64, 128, 256, 512, 1024]
    for i in shape_list:
        mats = matrix_gen(i)
        to_file(mats, os.path.join(args.dir, "%d/" % i))

if __name__ == "__main__":
    main()