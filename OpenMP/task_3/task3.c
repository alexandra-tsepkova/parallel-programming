#include <stdio.h>
#include "omp.h"

int main() {
    int N = 100000;
    // open the file where we will write the result
    FILE *output = fopen("output.txt", "w");
    if (!output){
        printf("Cannot open the result file!\n");
    }
    // initialize arrays a and b
    unsigned a[N];
    double b[N];
    for (unsigned i = 0; i < N; ++i) {
        a[i] = i;
        b[i] = 0;
    }

    // count all elems of b array except for boundary cases, because they don't have either a[i-1] or a[i+1]
    // use schedule to split the counting
#pragma omp parallel for schedule(auto)
    for (unsigned i = 1; i < N - 1; i++) {
        b[i] = (a[i - 1] * a[i] * a[i + 1]) / 3.0;
    }
    // make boundary values (b[0] and b[99999] equal to their neighbors (b[1] and b[99998]), because there is no clear rule for them
#pragma omp single
    {
        b[0] = b[1];
        b[N - 1] = b[N - 2];
    }
    // write result to file beautifully
    for (unsigned i = 0; i < N; ++i){
        fprintf(output, "b[%u] = %.2lf\n", i, b[i]);
    }

    return 0;
}