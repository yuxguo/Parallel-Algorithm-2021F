#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int my_allgater_mpi(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);

int main(int argc, char **argv)
{
    int rank, size;
    int my_value;
    int *my_recvbuf;
    MPI_Status status;
    clock_t start, end;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    my_value = rank;
    my_recvbuf = (int *)malloc(size * 1 * sizeof(int));

    start = clock();
    MPI_Allgather(&my_value, 1, MPI_INT, my_recvbuf, 1, MPI_INT, MPI_COMM_WORLD);
    // my_allgater_mpi(&my_value, 1, MPI_INT, my_recvbuf, 1, MPI_INT, MPI_COMM_WORLD);
    end = clock();
    printf("\nAfter allgather (rank %d), time:%.3lf ms\n", rank, (double)(end - start) / CLOCKS_PER_SEC * 1e3);
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", my_recvbuf[i]);
    }

    free(my_recvbuf);

    return 0;
}

int my_allgater_mpi(
    const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    for (int i = 0; i < size; ++i)
    {
        if (i != rank)
        {
            MPI_Send(sendbuf, sendcount, sendtype, i, 0, comm);
        }
    }
    for (int i = 0; i < size; ++i)
    {
        if (i != rank)
        {
            MPI_Recv(recvbuf + i * recvcount * sizeof(recvtype), recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
        }
    }
    memcpy(recvbuf + rank * recvcount * sizeof(recvtype), sendbuf, sendcount * sizeof(sendtype));
    return 0;
}
