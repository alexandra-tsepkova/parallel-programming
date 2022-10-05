#include <stdio.h>
#include "omp.h"

int main(){
    // in parallel section we save a number of the current thread into a variable num
    // then we print this number with a hello message :)

    #pragma omp parallel
    {
        int num;
        num = omp_get_thread_num();
        printf("Hello, I'm thread number %d\n", num);
    }
    return 0;
}