#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
#pragma omp parallel private(inicial_x, final_x, inicial_y, final_y) reduction(+ : f)

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
    int intervals[3] = {1000, 10000, 100000};
    int thread_counts[4] = {1, 2, 4, 8};

    for (int t = 0; t < 4; t++)
    {
        omp_set_num_threads(thread_counts[t]);
        printf("Executando com %d threads:\n", thread_counts[t]);

        for (int i = 0; i < 3; i++)
        {
            clock_t start = clock();
            double resultado = trapezio(intervals[i]);
            clock_t end = clock();
            double tempo = ((double)(end - start)) / CLOCKS_PER_SEC;

            printf("Intervalos: %d, Resultado: %f, Tempo: %f segundos\n",
                   intervals[i], resultado, tempo);
        }
        printf("\n");
    }

    return 0;
}
