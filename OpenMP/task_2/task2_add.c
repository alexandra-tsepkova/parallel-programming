#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

int main(int argc, char **argv){
    // N is passed as argument while launching the program
    int N = atoi(argv[1]);
    int global_sum = 0;

// here we use schedule to split sum between threads
// we use reduction for safety
#pragma omp parallel for schedule(auto) reduction(+:global_sum)

    for(int i = 1; i < N + 1; i++){
        global_sum += i;
    }

    printf("Sum of numbers from 1 to %d = %d\n", N, global_sum);
    return 0;
}
