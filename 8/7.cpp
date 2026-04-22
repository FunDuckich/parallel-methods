#include <stdio.h>
#include <stddef.h>
#include <mpi.h>

struct Particle {
    double x;
    double y;
    double z;
    int charge;
    double mass;
    char extra[8];
};

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
        MPI_Datatype raw_particle_type;
        MPI_Datatype particle_type;
        int block_lengths[] = {3, 1, 1, 8};
        MPI_Aint offsets[] = {
            offsetof(Particle, x),
            offsetof(Particle, charge),
            offsetof(Particle, mass),
            offsetof(Particle, extra)
        };
        MPI_Datatype types[] = {MPI_DOUBLE, MPI_INT, MPI_DOUBLE, MPI_CHAR};

        MPI_Type_create_struct(4, block_lengths, offsets, types, &raw_particle_type);
        MPI_Type_create_resized(raw_particle_type, 0, sizeof(Particle), &particle_type);
        MPI_Type_commit(&particle_type);

        Particle particles[3];

        if (id == 0) {
            particles[0] = {1.0, 2.0, 3.0, 1, 1.67, "proton"};
            particles[1] = {4.0, 5.0, 6.0, -1, 0.91, "electro"};
            particles[2] = {7.0, 8.0, 9.0, 0, 3.14, "neutral"};

            MPI_Send(particles, 3, particle_type, 1, 0, MPI_COMM_WORLD);

            printf("process 0 sent particles\n");
        }

        if (id == 1) {
            MPI_Recv(particles, 3, particle_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < 3; i++) {
                printf("particle %d: %.1f %.1f %.1f charge %d mass %.2f extra %.8s\n", i + 1, particles[i].x, particles[i].y, particles[i].z, particles[i].charge, particles[i].mass, particles[i].extra);
            }
        }

        MPI_Type_free(&particle_type);
        MPI_Type_free(&raw_particle_type);
        MPI_Finalize();
    }
}
