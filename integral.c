#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define intervalo 1.5

double seno(double x, double y)
{
    double valor_interno = x * x + y * y;
    return sin(valor_interno);
}

double trapezio(int tamanho_intervalo)
{
    double delta = intervalo / tamanho_intervalo;
    double inicial_x = 0, inicial_y = 0;
    double final_y = delta;
    double final_x = delta;
    double f = 0;

    while (final_x <= intervalo)
    {
        while (final_y <= intervalo)
        {
            f += (delta / 2) * (seno(inicial_x, inicial_y) + seno(inicial_x, final_y));
            inicial_y = final_y;
            final_y = final_y + delta;
        }
        inicial_x = final_x;
        final_x = final_x + delta;
    }
    return f;
}

int main(int argc, char *argv[])
{
    int nthreads, tid;
    int intervals[3] = {1000, 10000, 100000};

#pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        printf("Hello World from thread %d\n", tid);
#pragma omp barrier // Sincroniza as threads
#pragma omp master
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
    }
}
