/******************************************************************************
 * ЗАДАНИЕ: bugged4.c
 * ОПИСАНИЕ:
 *   Очень простая программа параллельных манипуляций с двумерным массивом
 *   зависимо от количества исполнителей... но с segmentation fault.
 ******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

double **allocate_array(){
    double **a = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++)
        a[i] = (double *)malloc(N * sizeof(double ));
    return a;
}

void free_array(double **a){
    for (int i = 0; i < N; i++)
        free(a[i]);
    free(a);
}

int main (int argc, char *argv[])
{
    int nthreads, tid, i, j;

    // массив слишком большой для аллокации на стеке. Размер стека ~10Mb, а размер массива ~16Mb :(
    // нужно аллоцировать его на куче
    double **a = allocate_array();

#pragma omp parallel shared(nthreads, a) private(i, j, tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++) {
                a[i][j] = tid + i + j;
            }
        printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);
    }

    // надо убрать за собой
    free_array(a);
}
