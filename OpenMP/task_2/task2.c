#include <stdio.h>
#include "omp.h"

int main() {
    int N;
    printf("Enter N: ");
    scanf("%d", &N);
    int global_sum = 0, local_sum = 0;
    // we save amount of numbers that each thread has to sum in a variable n_each_thread
    // if N % num_threads is not 0, a single thread will add the leftover numbers to global_sum
    int i, num_threads, n_each_thread;

// local sum is private because each thread has different numbers to sum and put there
#pragma omp parallel private(local_sum)
    {
        num_threads = omp_get_num_threads();
        n_each_thread = (int) (N / num_threads);
        int num = omp_get_thread_num();

        // local sum is counted here by different threads
        #pragma omp parallel for
        for (i = n_each_thread * num + 1; i < n_each_thread * (num + 1) + 1; i++) {
            local_sum += i;
        }

        // local sum gets added to global sum in critical section to avoid race condition
        #pragma omp critical
        {
            global_sum += local_sum;
        }

        // here we add leftover numbers to global sum by single thread
        // for example, we have N=10 and 4 threads, each thread sums 2 numbers, so 9 and 10 are left out
        // and here these 9 and 10 would be added
        #pragma omp single
        {
            if (N % num_threads != 0) {
                for (i = N - N % num_threads + 1; i < N + 1; i++) {
                    global_sum += i;
                }
            }
        }
    }

    printf("Sum of numbers from 1 to %d = %d\n", N, global_sum);
    return 0;
}