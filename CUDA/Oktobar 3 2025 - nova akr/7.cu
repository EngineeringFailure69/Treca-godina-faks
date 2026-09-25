%%cuda
#include <stdio.h>
#include <cuda_runtime.h>

#define BLOCK_SIZE 256


__global__ void filtriraj_i_detektuj(float *A, float *B, int *C, int *broj_potresa, int n, int m, float T)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    int stride = blockDim.x * gridDim.x;


    while(tid < n)
    {
        float suma = 0.0f;
        int broj = 0;


        int pola = m / 2;


        // klizni prozor
        for(int j = tid - pola; j <= tid + pola; j++)
        {
            if(j >= 0 && j < n)
            {
                suma += A[j];
                broj++;
            }
        }


        B[tid] = suma / broj;



        // detekcija potresa
        if(B[tid] > T)
        {
            int pozicija = atomicAdd(broj_potresa,1);
            C[pozicija] = tid;
        }


        tid += stride;
    }
}



int main()
{
    float A[] = {1,2,3,2,1,2,3,15,20,18,4,3,2,1,2};

    int n = sizeof(A)/sizeof(float);

    int m = 3;       // sirina prozora
    float T = 10.0;  // prag detekcije

    float *d_A;
    float *d_B;
    int *d_C;
    int *d_broj;

    cudaMalloc(&d_A,n*sizeof(float));
    cudaMalloc(&d_B,n*sizeof(float));
    cudaMalloc(&d_C,n*sizeof(int));
    cudaMalloc(&d_broj,sizeof(int));

    int nula = 0;

    cudaMemcpy(d_A,A,n*sizeof(float), cudaMemcpyHostToDevice);

    cudaMemcpy(d_broj,&nula,sizeof(int), cudaMemcpyHostToDevice);

    int grid = (n + BLOCK_SIZE - 1)/BLOCK_SIZE;

    filtriraj_i_detektuj<<<grid,BLOCK_SIZE>>>(d_A, d_B, d_C, d_broj, n, m, T);

    cudaDeviceSynchronize();

    float B[n];
    int C[n];
    int broj;

    cudaMemcpy(B,d_B,n*sizeof(float), cudaMemcpyDeviceToHost);

    cudaMemcpy(C,d_C,n*sizeof(int), cudaMemcpyDeviceToHost);

    cudaMemcpy(&broj,d_broj,sizeof(int), cudaMemcpyDeviceToHost);

    printf("Filtrirani niz B:\n");

    for(int i=0;i<n;i++)
    {
        printf("%.2f ",B[i]);
    }


    printf("\n\nDetektovani zemljotresi:\n");


    for(int i=0;i<broj;i++)
    {
        printf("Vreme: %d\n",C[i]);
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_broj);

    return 0;
}
