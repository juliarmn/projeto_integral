#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVALO 1.5
#define NOME_ARQUIVO "resultados29.csv"

double seno(long double x, long double y) {
    return sin(x * x + y * y);
}

double trapezio(int nx, int ny, int num_threads) {
    double hx = INTERVALO / nx;
    double hy = INTERVALO / ny;
    double integral = 0.0;

    omp_set_num_threads(num_threads);

#pragma omp parallel for reduction(+:integral)
    for (int i = 0; i <= nx; i++) {
        double x = i * hx;
        for (int j = 0; j <= ny; j++) {
            double y = j * hy;
            double peso = 1.0;

            if (i == 0 || i == nx) peso *= 0.5;
            if (j == 0 || j == ny) peso *= 0.5;

            integral += peso * seno(x, y);
        }
    }

    integral *= hx * hy;
    return integral;
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen(NOME_ARQUIVO, "a");
    if (argc != 3) {
        printf("Uso: %s <n_intervals_x> <n_intervals_y>\n", argv[0]);
        return 1;
    }

    int n_intervals_x = atoi(argv[1]);
    int n_intervals_y = atoi(argv[2]);

    double start_time;
    double resultado;
    double end_time;
    int num_threads = 1;

    char *env_threads = getenv("OMP_NUM_THREADS");
    if (env_threads != NULL) {
        num_threads = atoi(env_threads);
        printf("Número de threads: %d\n", num_threads);
    } else
        printf("Variável de ambiente OMP_NUM_THREADS não definida. Usando 1 thread.\n");

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "threads,intervalo_x,intervalo_y,resultado,tempo\n");
    }

    start_time = omp_get_wtime();
    resultado = trapezio(n_intervals_x, n_intervals_y, num_threads);
    end_time = omp_get_wtime();

    printf("Resultado com %d intervalos em x e %d intervalos em y: %.10f\n", n_intervals_x, n_intervals_y, resultado);
    printf("Tempo de execução: %.6f segundos\n", end_time - start_time);
    fprintf(fp, "%d,%d,%d,%.10f,%.6f\n", num_threads, n_intervals_x, n_intervals_y, resultado, end_time - start_time);
    fclose(fp);
    return 0;
}
