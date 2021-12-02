DATA_ROOT="./data"

if [ $# != 2 ] ; then
echo "Usage: bash run.sh NUM_OF_PROCS OMP_NUM_THREADS";
exit 1;
fi
for i in {64,}; do
    mpirun -np $1 -genv OMP_NUM_THREADS $2 -f /home/pp11/mpi_share/mpi_config ./LU $DATA_ROOT/$i/A.txt;
done;
