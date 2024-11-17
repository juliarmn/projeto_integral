#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVALO 1.5
#define THREADS_PER_BLOCK 512
#define NOME_ARQUIVO "resultados_cuda.csv"

__device__ double seno(double x, double y)
{
    return sin(x * x + y * y);
}

__global__ void trapezio_kernel(double hx, double hy, int nx, int ny, float *integral_d)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int total_threads = gridDim.x * blockDim.x;

    float local_integral = 0.0f;
    for (int i = idx; i <= nx; i += total_threads)
    {
        double x = i * hx;
        for (int j = 0; j <= ny; j++)
        {
            double y = j * hy;
            double peso = 1.0;

            if (i == 0 || i == nx)
                peso *= 0.5;
            if (j == 0 || j == ny)
                peso *= 0.5;

            local_integral += peso * seno(x, y);
        }
    }

    atomicAdd(integral_d, local_integral);
}

double trapezio_gpu(int nx, int ny, int num_blocks)
{
    double hx = INTERVALO / nx;
    double hy = INTERVALO / ny;
    float *integral_d;
    float integral_h = 0.0f;

    cudaMalloc((void **)&integral_d, sizeof(float));
    cudaMemcpy(integral_d, &integral_h, sizeof(float), cudaMemcpyHostToDevice);

    trapezio_kernel<<<num_blocks, THREADS_PER_BLOCK>>>(hx, hy, nx, ny, integral_d);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        printf("CUDA error: %s\n", cudaGetErrorString(err));
    }

    cudaMemcpy(&integral_h, integral_d, sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(integral_d);

    integral_h *= hx * hy;

    return integral_h;
}

int main()
{
    int intervalos_x[] = {1000, 10000, 100000};
    int intervalos_y[] = {1000, 10000, 100000};
    int blocos[] = {10, 100, 1000};

    FILE *fp = fopen(NOME_ARQUIVO, "w");
    if (!fp)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    fprintf(fp, "blocos,intervalo_x,intervalo_y,resultado,tempo\n");

    for (int b = 0; b < 3; b++)
    {
        for (int ix = 0; ix < 3; ix++)
        {
            for (int iy = 0; iy < 3; iy++)
            {
                int nx = intervalos_x[ix];
                int ny = intervalos_y[iy];
                int num_blocks = blocos[b];

                printf("Executando para %d blocos, %d intervalos em x e %d intervalos em y...\n",
                       num_blocks, nx, ny);

                cudaEvent_t start, stop;
                cudaEventCreate(&start);
                cudaEventCreate(&stop);

                cudaEventRecord(start, 0);
                double resultado = trapezio_gpu(nx, ny, num_blocks);
                cudaEventRecord(stop, 0);

                cudaEventSynchronize(stop);
                float milliseconds = 0;
                cudaEventElapsedTime(&milliseconds, start, stop);

                printf("Resultado: %.10f, Tempo: %.6f segundos\n", resultado, milliseconds / 1000.0);
                fprintf(fp, "%d,%d,%d,%.10f,%.6f\n", num_blocks, nx, ny, resultado, milliseconds / 1000.0);

                cudaEventDestroy(start);
                cudaEventDestroy(stop);
            }
        }
    }

    fclose(fp);
    return 0;
}
