#include <stdio.h>
#include <omp.h>

int global_var;

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        #pragma omp critical
        {
            global_var = id;
            printf("thread %d wrote %d to global_var\n", id, global_var);
        }
    }
}