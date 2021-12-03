DATA_ROOT="./data"

if [ $# != 1 ] ; then
echo "Usage: bash run.sh NUM_OF_PROCS";
exit 1;
fi
for i in {64,}; do
    mpirun -np $1 -genv -f /home/pp11/mpi_share/mpi_config ./summa $DATA_ROOT/$i/AB.txt;
done;
