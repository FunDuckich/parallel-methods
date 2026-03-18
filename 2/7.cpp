#include <stdio.h>
#include <omp.h>

int main() {
    int results[64];
    int count = 0;

    #pragma omp parallel shared(results, count)
    {
        int id = omp_get_thread_num();
        results[id] = id;

        #pragma omp master
        {
            count = omp_get_num_threads();
        }
    }

    for (int i = 0; i < count; i++) {
        printf("array[%d] = %d\n", i, results[i]);
    }
}