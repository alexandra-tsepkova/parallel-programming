#include <stdio.h>
#include "omp.h"

int main(){
    // a shared variable i is used as a counter to print nums of threads in reverse order
    // in parallel section we save a number of the current thread into a variable num
    // in a loop all threads wait for their turn to print their number
    // when their time comes they subtract 1 from the counter so that the next thread gets out of the loop

    int i = 0;
    #pragma omp parallel
    {
        int num;
        i = omp_get_num_threads() - 1;
        num = omp_get_thread_num();

        while(num != i){
        }

        printf("Hello, I'm thread number %d\n", num);
        --i;

    }
    return 0;
}
