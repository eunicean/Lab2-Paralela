#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

int main() {
    int i;
    double *myarray = (double*) malloc(N * sizeof(double));
    if (myarray == NULL) {
        printf("Error: no se pudo asignar memoria\n");
        return 1;
    }

    for (i = 0; i < N; i++) {
        myarray[i] = i * 0.001;
    }

    omp_sched_t s_kind;
    int chunk;
    omp_get_schedule(&s_kind, &chunk);

    double start = omp_get_wtime();
    #pragma omp parallel for schedule(runtime)
    for (i = 0; i < N; i++) {
        myarray[i] = sqrt(myarray[i]);
    }
    double end = omp_get_wtime();

    printf("Schedule usado: %s, chunk=%d\n", (s_kind == omp_sched_static ? "static" : s_kind == omp_sched_dynamic ? "dynamic" : "guided"), chunk);
    printf("Hilos usados: %d\n", omp_get_max_threads());
    printf("Tiempo paralelo: %f segundos\n", end - start);

    free(myarray);
    return 0;
}
