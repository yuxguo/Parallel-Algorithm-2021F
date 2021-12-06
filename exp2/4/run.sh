if [ $# != 1 ] ; then
echo "Usage: bash run.sh OMP_NUM_THREADS";
exit 1;
fi

OMP_NUM_THREADS=$1 ./omp_conv
