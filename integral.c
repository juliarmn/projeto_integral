#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVALO 1.5

double seno(double x, double y)
{
    return sin(x * x + y * y);
}

double trapezio(int n_intervals_x, int n_intervals_y)
{
    double delta_x = INTERVALO / n_intervals_x;
    double delta_y = INTERVALO / n_intervals_y;
    double f = 0.0;

#pragma omp parallel reduction(+ : f)
    {
        double local_sum = 0.0;
        double x0, y0, x1, y1;

#pragma omp for collapse(2) schedule(static)
        for (int i = 0; i < n_intervals_x; i++)
        {
            for (int j = 0; j < n_intervals_y; j++)
            {
                x0 = i * delta_x;
                y0 = j * delta_y;
                x1 = (i + 1) * delta_x;
                y1 = (j + 1) * delta_y;

                local_sum += (delta_x * delta_y / 4.0) *
                             (seno(x0, y0) + seno(x1, y0) + seno(x0, y1) + seno(x1, y1));
            }
        }

        f += local_sum;
    }

    return f;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <n_intervals_x> <n_intervals_y>\n", argv[0]);
        return 1;
    }

    int n_intervals_x = atoi(argv[1]);
    int n_intervals_y = atoi(argv[2]);
    double start_time;
    double resultado;
    double end_time;

    char *env_threads = getenv("OMP_NUM_THREADS");
    if (env_threads != NULL)
    {
        int num_threads = atoi(env_threads);
        omp_set_num_threads(num_threads);
        printf("Número de threads: %d\n", num_threads);
    }
    else
    {
        printf("Variável de ambiente OMP_NUM_THREADS não definida. Usando 1 thread.\n");
        omp_set_num_threads(1);
    }

    start_time = omp_get_wtime();
    resultado = trapezio(n_intervals_x, n_intervals_y);
    end_time = omp_get_wtime();

    printf("Resultado com %d intervalos em x e %d intervalos em y: %.10f\n", n_intervals_x, n_intervals_y, resultado);
    printf("Tempo de execução: %.6f segundos\n", end_time - start_time);

    return 0;
}
