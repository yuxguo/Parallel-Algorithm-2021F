MPICC = mpicc

my_allgather_mpi: my_allgather_mpi.c
	$(MPICC) my_allgather_mpi.c -lm -o my_allgather_mpi

my_alltoall_mpi: my_alltoall_mpi.c
	$(MPICC) my_alltoall_mpi.c -lm -o my_alltoall_mpi

clean: 
	rm -f my_allgather_mpi my_alltoall_mpi
