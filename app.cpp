#include <stdio.h>
#include <stdlib.h>

#include <cuda_runtime.h>

extern "C" {
void do_transpose(dim3 gDim, dim3 bDim, float *Md, float *Bd, uint WIDTH);
void do_transpose2(float *M, uint WIDTH, uint HEIGHT);
}

void test1() {
    const int HEIGHT = 5; // 1024;
    const int WIDTH = 5;  // 1024;
    const int SIZE = WIDTH * HEIGHT * sizeof(float);
    // dim3 bDim(16, 16);
    dim3 bDim(3, 3);
    dim3 gDim(WIDTH / bDim.x, HEIGHT / bDim.y);
    float *M = new float[SIZE];
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        M[i] = i;
    }

    for (int i = 0; i < 3; i++) {
        printf("%f ", M[i]);
    }
    printf("\n-------------------\n");

    float *Md = NULL;
    cudaMalloc((void **)&Md, SIZE);
    cudaMemcpy(Md, M, SIZE, cudaMemcpyHostToDevice);
    float *Bd = NULL;
    cudaMalloc((void **)&Bd, SIZE);
    do_transpose(gDim, bDim, Md, Bd, WIDTH);
    cudaMemcpy(M, Bd, SIZE, cudaMemcpyDeviceToHost);

    for (int i = 0; i < 3; i++) {
        printf("%f ", M[i]);
    }

    delete (M);
    printf("\n*** done1 ***\n");
}

void test2() {
    const int HEIGHT = 5; // 1024;
    const int WIDTH = 5;  // 1024;
    const int SIZE = WIDTH * HEIGHT * sizeof(float);

    float *M = new float[SIZE];
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        M[i] = i;
    }

    for (int i = 0; i < 3; i++) {
        printf("%f ", M[i]);
    }
    printf("\n-------------------\n");

    do_transpose2(M, WIDTH, HEIGHT);

    for (int i = 0; i < 3; i++) {
        printf("%f ", M[i]);
    }

    delete (M);
    printf("\n*** done2 ***\n");
}

int main(int args, char **vargs) {
    test1();
    test2();

    return 0;
}
