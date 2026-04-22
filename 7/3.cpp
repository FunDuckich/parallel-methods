#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int send_value = id * 10;
    int recv_value = -1;
    int next = (id + 1) % count;
    int prev = (id - 1 + count) % count;

    MPI_Request requests[2];

    MPI_Irecv(&recv_value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &requests[0]);
    MPI_Isend(&send_value, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &requests[1]);

    MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

    printf("process %d sent %d to %d and received %d from %d\n", id, send_value, next, recv_value, prev);

    MPI_Finalize();
}
