#include <stdio.h>
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
        double value = 12.75;
        int buffer_size = MPI_BSEND_OVERHEAD + sizeof(double);
        char buffer[MPI_BSEND_OVERHEAD + sizeof(double)];

        if (id == 0) {
            void *attached_buffer;

            MPI_Buffer_attach(buffer, buffer_size);
            MPI_Bsend(&value, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Buffer_detach(&attached_buffer, &buffer_size);

            printf("process 0 sent %.2f\n", value);
        }

        if (id == 1) {
            MPI_Recv(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("process 1 received %.2f\n", value);
        }

        MPI_Finalize();
    }
}
