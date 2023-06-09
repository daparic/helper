#include <cuda_runtime.h>

static __global__ void transpose(float *in, float *out, uint width) {
    uint tx = blockIdx.x * blockDim.x + threadIdx.x;
    uint ty = blockIdx.y * blockDim.y + threadIdx.y;
    out[tx * width + ty] = in[ty * width + tx];
}

#ifdef __cplusplus
extern "C" {
#endif

void do_transpose(dim3 gDim, dim3 bDim, float *Md, float *Bd, uint WIDTH) {
    transpose<<<gDim, bDim>>>(Md, Bd, WIDTH);
}

void do_transpose2(float *M, uint WIDTH, uint HEIGHT) {
    const int SIZE = WIDTH * HEIGHT * sizeof(float);

    dim3 bDim(3, 3);
    dim3 gDim(WIDTH / bDim.x, HEIGHT / bDim.y);

    float *Md = NULL;
    cudaMalloc((void **)&Md, SIZE);
    cudaMemcpy(Md, M, SIZE, cudaMemcpyHostToDevice);
    float *Bd = NULL;
    cudaMalloc((void **)&Bd, SIZE);
    do_transpose(gDim, bDim, Md, Bd, WIDTH);
    cudaMemcpy(M, Bd, SIZE, cudaMemcpyDeviceToHost);

    // free(M);
}

#ifdef __cplusplus
}
#endif
