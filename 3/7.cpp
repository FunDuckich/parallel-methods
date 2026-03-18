#include <stdio.h>
#include <omp.h>

int main() {
    int n = 0;

    #pragma omp parallel sections lastprivate(n)
    {
        #pragma omp section
        {
            n = 1;
            printf("section 1: thread %d set n to %d\n", omp_get_thread_num(), n);
        }

        #pragma omp section
        {
            n = 2;
            printf("section 2: thread %d set n to %d\n", omp_get_thread_num(), n);
        }

        #pragma omp section
        {
            n = 3;
            printf("section 3: thread %d set n to %d\n", omp_get_thread_num(), n);
        }
    }

    printf("after parallel sections: final n is %d\n", n);
}