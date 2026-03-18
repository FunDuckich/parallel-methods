#include <stdio.h>
#include <omp.h>

int personal_id;
#pragma omp threadprivate(personal_id)

int main() {
    omp_set_dynamic(0);
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        personal_id = id + 100;
        printf("first region: thread %d saved value %d\n", id, personal_id);
    }

    printf("sequential part: main thread is doing something...\n");

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("second region: thread %d still has value %d\n", id, personal_id);
    }
}