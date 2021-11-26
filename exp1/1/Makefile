CC = gcc-11
MPICC = mpicc
add_omp_p35:
	$(CC) add_omp_p35.c -fopenmp -o add_omp_p35

add_omp_p36:
	$(CC) add_omp_p36.c -fopenmp -o add_omp_p36

add_mpi_p35:
	$(MPICC) add_mpi_p35.c -lm -o add_mpi_p35

add_mpi_p36:
	$(MPICC) add_mpi_p36.c -lm -o add_mpi_p36

clean:
	rm -f add_omp_p35 add_omp_p36 add_mpi_p35 add_mpi_p36
