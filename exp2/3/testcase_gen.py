import numpy as np;
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("--dir", type=str, required=True)
args = parser.parse_args()

def to_file(mats, dir):
    os.system("mkdir -p %s" % dir)
    R, A, B = mats
    with open(os.path.join(dir, "AB.txt"), "w") as f:
        f.write("%d\n" % A.shape[0])
        for i in range(A.shape[0]):
            for j in range(A.shape[1]):
                f.write("%.6f\t" % A[i, j])
            f.write("\n")
        f.write("%d\n" % B.shape[0])
        for i in range(B.shape[0]):
            for j in range(B.shape[1]):
                f.write("%.6f\t" % B[i, j])
            f.write("\n")
    with open(os.path.join(dir, "result.txt"), "w") as f:
        f.write("result:\n")
        for i in range(R.shape[0]):
            for j in range(R.shape[1]):
                f.write("%.6f\t" % R[i, j])
            f.write("\n")

def matrix_gen(n):
    A = np.random.rand(n, n)
    B = np.random.rand(n, n)
    return A @ B, A, B

def main():
    shape_list = [16, 32, 64, 128, 256, 512, 1024]
    for i in shape_list:
        mats = matrix_gen(i)
        to_file(mats, os.path.join(args.dir, "%d/" % i))

if __name__ == "__main__":
    main()