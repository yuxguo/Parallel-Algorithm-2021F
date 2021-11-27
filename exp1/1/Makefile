CC = gcc-11
MPICC = mpicc
add_omp_p35: add_omp_p35.c
	$(CC) add_omp_p35.c -fopenmp -o add_omp_p35

add_omp_p36: add_omp_p36.c
	$(CC) add_omp_p36.c -fopenmp -o add_omp_p36

add_mpi_p35: add_mpi_p35.c
	$(MPICC) add_mpi_p35.c -lm -o add_mpi_p35

add_mpi_p36: add_mpi_p36.c
	$(MPICC) add_mpi_p36.c -lm -o add_mpi_p36

clean:
	rm -f add_omp_p35 add_omp_p36 add_mpi_p35 add_mpi_p36
