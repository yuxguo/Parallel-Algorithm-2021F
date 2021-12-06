DATA_ROOT="./data"

if [ $# != 2 ] ; then
echo "Usage: bash run.sh OMP_NUM_THREADS SCALE";
exit 1;
fi
OMP_NUM_THREADS=$1 ./QR $DATA_ROOT/$2/A.txt;
