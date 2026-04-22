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
        int sizes[] = {0, 1, 16, 64, 256, 1024, 4096, 16384, 65536, 262144, 1048576, 4194304};
        int sizes_count = sizeof(sizes) / sizeof(sizes[0]);
        int max_size = sizes[sizes_count - 1];
        char *buffer = new char[max_size];

        double latency = 0.0;
        double max_bandwidth = 0.0;
        int best_size = 0;

        for (int i = 0; i < sizes_count; i++) {
            int size = sizes[i];
            int repeats = 1000;

            if (size >= 65536) {
                repeats = 200;
            }

            if (size >= 1048576) {
                repeats = 50;
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double start_time = MPI_Wtime();

            for (int j = 0; j < repeats; j++) {
                if (id == 0) {
                    MPI_Send(buffer, size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(buffer, size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                if (id == 1) {
                    MPI_Recv(buffer, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Send(buffer, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                }
            }

            double end_time = MPI_Wtime();
            double one_way_time = (end_time - start_time) / (2.0 * repeats);

            if (id == 0) {
                if (size == 0) {
                    latency = one_way_time;
                } else {
                    double bandwidth = size / one_way_time / (1024.0 * 1024.0);

                    if (bandwidth > max_bandwidth) {
                        max_bandwidth = bandwidth;
                        best_size = size;
                    }
                }
            }
        }

        if (id == 0) {
            printf("latency: %.3f microseconds\n", latency * 1000000.0);
            printf("max bandwidth: %.3f MB/s\n", max_bandwidth);
            printf("message size: %d bytes\n", best_size);
        }

        delete[] buffer;
        MPI_Finalize();
    }
}
