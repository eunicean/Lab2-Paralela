#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

int main(int argc, char const *argv[])
{
    double *myarray = (double*) malloc(N * sizeof(double));

    for (long i = 0; i < N; i++) {
        myarray[i] = 1.0;   
    }

    double suma = 0.0;
    double start = omp_get_wtime();
    
    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        #pragma omp critical
        {
            suma += myarray[i];
        }
    }

    double end = omp_get_wtime();
    printf("Critical - Suma: %.0f, Tiempo: %f s\n", suma, end - start);
    return 0;
}
