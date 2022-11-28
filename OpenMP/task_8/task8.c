#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

// For beautiful output of sorted array
void print_array(int *array, int n){
    for (int i = 0; i < n; ++i){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int *read_array_from_file(char* filename, int *n){
    FILE *input = fopen(filename, "r");
    fscanf(input, "%d", &(*n));
    int *array = (int*)malloc(*n * sizeof(int));
    for (int i = 0; i < *n; ++i){
        fscanf(input, "%d", &array[i]);
    }
    return array;
}

int *read_array_from_stdin(int *n){
    printf("\nInput amount of elements in array\n");
    scanf("%d", &(*n));
    int *array = malloc(*n * sizeof(int));
    for (int i = 0; i < *n; ++i){
        scanf ("%d", &array[i]);
    }
    return array;
}

// Helper function for quicksort
int partition(int *array, int l, int r){
    int div = array[(l + r) / 2], tmpl, tmpr;
    int i = l, j = r;
    while (i <= j){
        while(array[i] < div){
            i++;
        }
        while(array[j] > div){
            j--;
        }
        if (i >= j) break;
        tmpl = array[i];
        tmpr = array[j];
        array[i++] = tmpr;
        array[j--] = tmpl;
    }
    return j;
}

// Quicksort function. Recursive call of quicksort is in parallel section
// So each partition is processed in parallel
void quicksort(int *array, int l, int r){
    if (l < r){
        int q = partition(array, l, r);
#pragma omp parallel shared(array)
        {
            quicksort(array, l, q);
            quicksort(array, q + 1, r);
        }
    }
}


int main(int argc, char **argv){
    int n = 0;
    int *a;
    if (argc < 2) {
        printf("\nUsage: ./task8 <input condition>\n");
        exit(1);
    }

    // Choose mode: if 1, then read from file, if 2, then read from stdin
    printf("%s", argv[1]);
    if (strcmp(argv[1], "1") == 0){
        a = read_array_from_file("input.txt", &n);  // file name must be input.txt
    }
    else if (strcmp(argv[1], "2") == 0){
        a = read_array_from_stdin(&n);
    }
    else{  // if you input not 1 or 2, exit
        printf("\nAllowed values are either 1 (file input) or 2 (stdin)\n");
        exit(1);
    }

    quicksort(a, 0, n - 1);

    printf("\nSorted array: ");
    print_array(a, n);
    free(a);
    return 0;
}