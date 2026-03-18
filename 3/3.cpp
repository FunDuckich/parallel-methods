#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        if (id == 0) {
            int total = omp_get_num_threads();
            printf("thread 0 reports: total number of threads is %d\n", total);
        } else {
            printf("i am thread number %d\n", id);
        }
    }
}