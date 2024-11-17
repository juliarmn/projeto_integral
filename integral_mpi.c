#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVALO 1.5
#define NOME_ARQUIVO "resultados_mpi.csv"

double seno(long double x, long double y) {
    return sin(x * x + y * y);
}

double calcular_parte(int nx, int ny, double hx, double hy, int start, int end) {
    double integral = 0.0;

    for (int i = start; i < end; i++) {
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
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) {
            printf("Uso: %s <n_intervals_x> <n_intervals_y>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int nx = atoi(argv[1]);
    int ny = atoi(argv[2]);

    double hx = INTERVALO / nx;
    double hy = INTERVALO / ny;

    int linhas_por_processo = nx / size;
    int resto = nx % size;

    int start = rank * linhas_por_processo + (rank < resto ? rank : resto);
    int end = start + linhas_por_processo + (rank < resto);

    double start_time, end_time;
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    double integral_local = calcular_parte(nx, ny, hx, hy, start, end);

    double integral_total = 0.0;
    MPI_Reduce(&integral_local, &integral_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        end_time = MPI_Wtime();

        printf("Resultado com %d intervalos em x e %d intervalos em y: %.10f\n", nx, ny, integral_total);
        printf("Tempo de execução: %.6f segundos\n", end_time - start_time);

        FILE *fp = fopen(NOME_ARQUIVO, "a");
        if (!fp) {
            printf("Erro ao abrir o arquivo %s\n", NOME_ARQUIVO);
            MPI_Finalize();
            return 1;
        }

        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) {
            fprintf(fp, "processos,intervalo_x,intervalo_y,resultado,tempo\n");
        }
        fprintf(fp, "%d,%d,%d,%.10f,%.6f\n", size, nx, ny, integral_total, end_time - start_time);
        fclose(fp);
    }

    MPI_Finalize();
    return 0;
}
