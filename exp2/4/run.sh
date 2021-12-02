DATA_ROOT="./data"

if [ $# != 1 ] ; then
echo "Usage: bash run.sh OMP_NUM_THREADS";
exit 1;
fi
for i in {16,32,64,128,256,512,1024}; do
    OMP_NUM_THREADS=$1 ./QR $DATA_ROOT/$i/A.txt;
done;
