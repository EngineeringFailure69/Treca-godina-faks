%%cuda
#include<stdio.h>
#include<stdlib.h>
#include<cuda_runtime.h>

#define BLOCK_SIZE 256

__global__ void detektuj_otkucaje(int* ekg_signal, int* broj_otkucaja_srca, int n, float prag_alfa)
{
    __shared__ int hits[BLOCK_SIZE];

    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = blockDim.x * gridDim.x;

    hits[threadIdx.x] = 0;
    
    while(tid < n)
    {
        if(tid == 0)
        {
            if(ekg_signal[tid] > ekg_signal[tid + 1] && ekg_signal[tid] > prag_alfa)
                hits[threadIdx.x]++;
        }
        else if(tid == n-1)
        {
            if(ekg_signal[tid] > ekg_signal[tid - 1] && ekg_signal[tid] > prag_alfa)
                hits[threadIdx.x]++;
        }
        else if(tid > 0 && tid < n-1)
        {
            if(ekg_signal[tid] > ekg_signal[tid - 1] && ekg_signal[tid] > prag_alfa
               && ekg_signal[tid] > ekg_signal[tid + 1])
                hits[threadIdx.x]++;
        }
        tid += stride;
    }

    for(int i = blockDim.x / 2; i > 0; i /= 2)
    {
        if(threadIdx.x < i)
        {
            hits[threadIdx.x] += hits[threadIdx.x + i];
        }
        __syncthreads();
    }

    if(threadIdx.x == 0)
        atomicAdd(broj_otkucaja_srca, hits[0]);
}

int main()
{
    int ekg_signal[] = {9,2,7,3,1,6,2,8,4,1,9,2,4,8,1,9};
    int* broj_otkucaja_srca;
    float prag_alfa = 4.0f;  
    int nula = 0;
    int* cuda_ekg_signal;
    int n = sizeof(ekg_signal) / sizeof(int);

    cudaMalloc((void**)&cuda_ekg_signal, n * sizeof(int));
    cudaMalloc((void**)&broj_otkucaja_srca, sizeof(int));

    cudaMemcpy(cuda_ekg_signal, ekg_signal, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(broj_otkucaja_srca, &nula, sizeof(int), cudaMemcpyHostToDevice);

    int grid_size = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;

    detektuj_otkucaje<<<grid_size, BLOCK_SIZE>>>(cuda_ekg_signal, broj_otkucaja_srca,
                                                 n, prag_alfa);
    
    cudaDeviceSynchronize();
    
    int rezultat;

    cudaMemcpy(&rezultat, broj_otkucaja_srca, sizeof(int), cudaMemcpyDeviceToHost);

    printf("\nBroj detektovanih otkucaja: %d\n", rezultat);

    cudaFree(cuda_ekg_signal);
    cudaFree(broj_otkucaja_srca);
    return 0;
}