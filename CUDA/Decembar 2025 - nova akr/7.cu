%%cuda

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define BLOCK_SIZE 256

__global__ void detektujLokalneMaksimume(const int* S, int* B, int n, int k)
{
    extern __shared__ int sharedS[];
    int local_id = threadIdx.x;
    int block_start = blockIdx.x * blockDim.x;
    int tid = block_start + local_id;

    int shared_index = local_id + k;

    if(tid<n)
      sharedS[shared_index] = S[tid];
    
     if (local_id < k)
    {
        int left_index = block_start + local_id - k;

        if (left_index >= 0)
            sharedS[local_id] = S[left_index];
        else
            sharedS[local_id] = -2147483648;
    }

    if (local_id < k)
    {
        int right_index = block_start + blockDim.x + local_id;

        if (right_index < n)
            sharedS[blockDim.x + k + local_id] = S[right_index];
        else
            sharedS[blockDim.x + k + local_id] = -2147483648;
    }

    __syncthreads();

    if (tid < n)
    {
        if (tid < k || tid >= n - k)
        {
            B[tid] = 0;
        }
        else
        {
            bool lokalni_maksimum = true;

            for (int j = 1; j <= k; j++)
            {
                if (sharedS[shared_index] <= sharedS[shared_index - j] ||
                    sharedS[shared_index] <= sharedS[shared_index + j])
                {
                    lokalni_maksimum = false;
                    break;
                }
            }

            if (lokalni_maksimum)
                B[tid] = 1;
            else
                B[tid] = 0;
        }
    }
}

int main()
{
    int S[] = {9, 2, 7, 3, 1, 6, 2, 8, 4, 1, 9, 2, 4, 8, 1, 9};
    int n = sizeof(S) / sizeof(int);
    int k = 1;
    int B[n];
    int* cuda_signal;
    int* B_result;

    cudaMalloc((void**)&cuda_signal, n*sizeof(int));
    cudaMalloc((void**)&B_result, n*sizeof(int));

    cudaMemcpy(cuda_signal, S, n*sizeof(int), cudaMemcpyHostToDevice);
    int grid_size = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int shared_memory_size = (BLOCK_SIZE + 2*k) * sizeof(int);

    detektujLokalneMaksimume<<<grid_size, BLOCK_SIZE, shared_memory_size>>>(
        cuda_signal, B_result, n, k);
    
    cudaDeviceSynchronize();

    cudaMemcpy(B, B_result, n*sizeof(int), cudaMemcpyDeviceToHost);

    printf("S: ");

    for (int i = 0; i < n; i++)
        printf("%d ", S[i]);

    printf("\n");

    printf("B: ");

    for (int i = 0; i < n; i++)
        printf("%d ", B[i]);

    printf("\n");

    cudaFree(cuda_signal);
    cudaFree(B_result);
    return 0;
}