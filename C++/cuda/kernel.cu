
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
#include "utils.h"
//630188182
#define SIZEN 630188182 
#define MAX_LENGTH 7 
#define CHARSETLEN 26 


#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}


__device__ __constant__ unsigned char cudaCharSet[95];
__device__ unsigned char correctPass[MAX_LENGTH];

__global__ void bruteForce(unsigned char* prefixes, int length, int offset_start, int offeset_end, int prefix_to_calc, uint v1, uint v2, uint v3, uint v4, unsigned char* password_d)
{
    int blockId = blockIdx.x + blockIdx.y * gridDim.x;
    int local_threadId = threadIdx.x + threadIdx.y * gridDim.x;
    int threadId = blockId * (blockDim.x * blockDim.y)
        + (threadIdx.y * blockDim.x) + threadIdx.x;
    //prefixes[offset + threadId + length - 1] = 'a';

    if (blockId < prefix_to_calc - 1 && local_threadId < CHARSETLEN - 1 && password_d[0]== '\0') {
        //Need to optimize 7758650592 
        memcpy(prefixes + (offeset_end + threadId) * MAX_LENGTH, prefixes + (offset_start + blockId) * MAX_LENGTH, (length - 1) * sizeof(char));
        prefixes[(offeset_end + threadId) * MAX_LENGTH + length - 1] = cudaCharSet[local_threadId];
        uint c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        md5_vfy(prefixes + (offeset_end + threadId) * MAX_LENGTH, length, &c1, &c2, &c3, &c4);
        if (c1 == v1 && c2 == v2 && c3 == v3 && c4 == v4)
        {
            memcpy(password_d, prefixes + (offeset_end + threadId) * MAX_LENGTH, length * sizeof(char));
        }
    }
}

int main(int argc, unsigned char* argv[])
{
    char charset[] = "abcdefghijklmnopqrstuvwxyz";
    //af94ffbb0e815172b1160d4b58a3ece3 -> imanol
    //172346606e1d24062e891d537e917a90 -> lolol 
    unsigned char hash[33] = "eba021d91b44a97dec2588bbea58a447";
    unsigned char password[MAX_LENGTH], * password_d;


    uint v1, v2, v3, v4;
    md5_to_ints(hash, &v1, &v2, &v3, &v4);
    //  printf("%i %i %i %i", v1, v2, v3, v4);
    cudaMemcpyToSymbol(cudaCharSet, &charset, CHARSETLEN, 0, cudaMemcpyHostToDevice);

    double ms;
    printf("Starting...\n");
    ms = 0;
    for (int h = 0; h < N; h++) {
        long prefix_n = 26;
        int length = 5;
        long offset_start = 0;
        long offest_end = 26;
        unsigned char* prefixes_d, * prefixes;
        prefixes = (unsigned char*)malloc(SIZEN * sizeof(char) * MAX_LENGTH);
        memset(prefixes, '\0', SIZEN * sizeof(unsigned char) * MAX_LENGTH);
        memset(password, '\0', sizeof(unsigned char) * MAX_LENGTH);

        //load first prefix
        for (int i = 0; i < CHARSETLEN; i++) prefixes[i * MAX_LENGTH] = charset[i];
        cudaMalloc((void**)&prefixes_d, SIZEN * sizeof(unsigned char) * MAX_LENGTH);
        cudaMalloc((void**)&password_d, MAX_LENGTH * sizeof(unsigned char));

        cudaMemcpy(prefixes_d, prefixes, CHARSETLEN * MAX_LENGTH * sizeof(unsigned char), cudaMemcpyHostToDevice);
        cudaMemcpy(password_d, password, MAX_LENGTH * sizeof(unsigned char), cudaMemcpyHostToDevice);

        start_timer();
        for (int i = 2; i <= length; i++) {
            long dim = (int)ceil(sqrt((float)prefix_n));
            dim3 grid_dim(dim, dim);
            prefix_n = prefix_n * CHARSETLEN;
            bruteForce << < grid_dim, 26 >> > (prefixes_d, i, offset_start, offest_end, prefix_n, v1, v2, v3, v4, password_d);
            cudaDeviceSynchronize();
            gpuErrchk(cudaPeekAtLastError());
            gpuErrchk(cudaDeviceSynchronize());
            cudaMemcpy(password, password_d, MAX_LENGTH * sizeof(unsigned char), cudaMemcpyDeviceToHost);
            if (password[0] != '\0')
            {
                printf("We found %s as %s  ", hash, password);
                break;
            }

            offset_start = offest_end;
            offest_end = offset_start + prefix_n;

        }
        ms += stop_timer();
        free(prefixes);
        cudaFree(prefixes_d);
        cudaFree(password_d);
    }
    printf("avg time = %.5lf ms\n", (ms / N));


}