#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        printf("message 1, thread %d\n", id);

        #pragma omp barrier

        printf("message 2, thread %d\n", id);
    }
}