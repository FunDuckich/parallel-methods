#include <stdio.h>

int main() {
#ifdef _OPENMP
    printf("openmp working\n");
    
    int year = _OPENMP / 100;
    int month = _OPENMP % 100;
    
    printf("year %d, month %d\n", year, month);
#else 
    printf("openmp is not supported\n");
#endif
}