DATA_ROOT="./data"

if [ $# != 2 ] ; then
echo "Usage: bash run.sh NUM_OF_PROCS SCALE";
exit 1;
fi

mpirun -np $1 -genv -f /home/pp11/mpi_share/mpi_config ./summa $DATA_ROOT/$2/AB.txt;
