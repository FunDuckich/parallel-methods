#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        printf("thread %d reached the block\n", id);

        #pragma omp single nowait
        {
            printf("thread %d is doing the single work\n", id);
        }

        printf("thread %d passed the block without waiting\n", id);
    }
}