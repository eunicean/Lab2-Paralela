#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

int main(int argc, char const *argv[])
{
    int i;
    double *myarray = (double*) malloc(N * sizeof(double));

    for (i = 0; i < N; i++) {
        myarray[i] = i * 0.001;
    }

    double start = omp_get_wtime();
    for (i = 0; i < N; i++) {
        myarray[i] = sqrt(myarray[i]);
    }
    double end = omp_get_wtime();
    printf("Tiempo secuencial: %f segundos\n", end - start);
    free(myarray);
    return 0;
}
