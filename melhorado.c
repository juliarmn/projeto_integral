#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INTERVALO 1.5

double seno(double x, double y)
{
    double valor_interno = x * x + y * y;
    return sin(valor_interno);
}

double trapezio(int tamanho_intervalo_x, int tamanho_intervalo_y)
{
    double delta_x = INTERVALO / tamanho_intervalo_x;
    double delta_y = INTERVALO / tamanho_intervalo_y;
    double f = 0.0;

    #pragma omp parallel for collapse(2) reduction(+:f)
    for (int i = 0; i < tamanho_intervalo_x; i++)
    {
        double x = i * delta_x;
        for (int j = 0; j < tamanho_intervalo_y; j++)
        {
            double y = j * delta_y;
            f += (delta_x * delta_y / 4.0) * 
                (seno(x, y) + seno(x + delta_x, y) +
                 seno(x, y + delta_y) + seno(x + delta_x, y + delta_y));
        }
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
            double resultado = trapezio(intervals[i], intervals[i]);
            clock_t end = clock();
            double tempo = ((double)(end - start)) / CLOCKS_PER_SEC;

            printf("Intervalos: %d, Resultado: %f, Tempo: %f segundos\n",
                   intervals[i], resultado, tempo);
        }
        printf("\n");
    }

    return 0;
}
