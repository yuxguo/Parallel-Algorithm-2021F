DATA_ROOT="./data"

if [ $# != 3 ] ; then
echo "Usage: bash run.sh NUM_OF_PROCS OMP_NUM_THREADS SCALE";
exit 1;
fi

mpirun -np $1 -genv OMP_NUM_THREADS $2 -f /home/pp11/mpi_share/mpi_config ./LU $DATA_ROOT/$3/A.txt;
