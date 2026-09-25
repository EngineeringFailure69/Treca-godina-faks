%%cuda

#include<stdio.h>
#include<stdlib.h>
#include<cuda_runtime.h>

#define BLOCK_SIZE 256

__global__ void proveri_vlaznost(float* A, int* rezultat, int M, int N)        
{
    int tid = threadIdx.x + blockDim.x * blockIdx.x;
    int stride = blockDim.x * gridDim.x;

    while(tid < M)
    {
        int poslednje_zalivanje = 0;
        for(int j=1; j<N; j++)
        {
            float trenutna_vlaznost = A[tid * N + j];
            float prethoda_vlaznost = A[tid * N + j - 1];
            if(trenutna_vlaznost < 0.5f && trenutna_vlaznost < prethoda_vlaznost){}
            else
                poslednje_zalivanje = j; //biljka jeste zalivena
        }
        int proteklo = N - 1 - poslednje_zalivanje;
        if(proteklo >= 5)
            rezultat[tid] = proteklo;
        else
            rezultat[tid] = 0;
        
        tid += stride;
    }
}

int main()
{
    float A[5][8] = 
    {
        //h0   h1   h2   h3   h4   h5   h6   h7
        {0.9, 0.8, 0.7, 0.6, 0.4, 0.3, 0.2, 0.1}, // biljka 0
        {0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1}, // biljka 1
        {0.9, 0.8, 0.7, 0.6, 0.7, 0.8, 0.7, 0.6}, // biljka 2
        {0.6, 0.5, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0}, // biljka 3
        {0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.8}  // biljka 4
    };
    int M = 5, N = 8;
    int* cuda_rezultat;
    float* cuda_A;

    cudaMalloc((void**)&cuda_rezultat, M*sizeof(int));
    cudaMalloc((void**)&cuda_A, M*N*sizeof(float));

    cudaMemcpy(cuda_A, A, M*N*sizeof(float), cudaMemcpyHostToDevice);

    int grid_size = (M + BLOCK_SIZE - 1) / BLOCK_SIZE;

    proveri_vlaznost<<<grid_size, BLOCK_SIZE>>>(cuda_A, cuda_rezultat, M, N);

    cudaDeviceSynchronize(); 

    int rezultat[M];

    cudaMemcpy(rezultat, cuda_rezultat, M*sizeof(int), cudaMemcpyDeviceToHost);

    for(int i=0; i<M; i++)
    {
        if(rezultat[i]>0)
            printf("Biljka %d nije zalivana %d sati\n", i, rezultat[i]);
        else
            printf("Biljka %d je OK\n", i);
    }

    cudaFree(cuda_rezultat);
    cudaFree(cuda_A);
    return 0;
}