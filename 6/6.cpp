#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int id;
    int count;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    if (count < 2) {
        if (id == 0) {
            printf("need at least 2 processes\n");
        }

        MPI_Finalize();
    } else {
        if (id == 0) {
            char message[] = "message with unknown length";
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

            printf("process 0 sent message\n");
        }

        if (id == 1) {
            MPI_Status status;
            int length;

            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &length);

            char *message = new char[length];
            MPI_Recv(message, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("message length is %d\n", length - 1);
            printf("process 1 received: %s\n", message);

            delete[] message;
        }

        MPI_Finalize();
    }
}
