#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

// this function creates matrix size1 x size2 and returns it
int **create_matrix(int size1, int size2){
    int** matrix = (int **)calloc(size1, sizeof(int *));

    for (int i = 0; i < size1; i++)
        matrix[i] = (int *) calloc(size2, sizeof(int));

    return matrix;
}

// this function fills matrix size1 x size2 with random numbers from 0 to 9
void set_matrix_with_random_numbers(int size1, int size2, int **matrix){
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

// this function prints given matrix size1 x size2 to file filename
void print_matrix_to_file(char* filename, int size1, int size2, int **matrix){
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < size1; ++i){
        for (int j = 0; j < size2; ++j){
            fprintf(f,"%d ", matrix[i][j]);
        }
        fprintf(f, "\n");
    }
    return;
}

int main(){
    srand(time(0));
    int **matrix_1, **matrix_2, **res_matrix;
    int i, j, k;
    // set sizes for matrices. matrix_1 is size1 x size2, matrix_2 is size2 x size3
    // 1 is added to make sure size is never zero, so sizes can be from 1 to 20
    unsigned size1 = rand() % 20 + 1, size2 = rand() % 20 + 1, size3 = rand() % 20 + 1;


    // create and set matrices. on creation all matrices are set to zeros
    // then matrix_1 and matrix_2 are filled with random numbers
    matrix_1 = create_matrix(size1, size2);
    set_matrix_with_random_numbers(size1, size2, matrix_1);
    matrix_2 = create_matrix(size2, size3);
    set_matrix_with_random_numbers(size2, size3, matrix_2);
    res_matrix = create_matrix(size1, size3);

// schedule is needed to split multiplication between threads
// counters i j and k are private because each thread increases them, so they cannot be shared
#pragma omp parallel for schedule(auto) private(i, j, k)
    for (i = 0; i < size1; ++i){
        for(j = 0; j < size2; ++j){
            for (k = 0; k < size3; ++k){
                res_matrix[i][k] += matrix_1[i][j] * matrix_2[j][k];
            }
        }
    }

    // print generated matrices to files matrix_1.txt and matrix_2.txt
    // print result of multiplication to output.txt
    print_matrix_to_file("matrix_1.txt", size1, size2, matrix_1);
    print_matrix_to_file("matrix_2.txt", size2, size3, matrix_2);
    print_matrix_to_file("output.txt", size1, size3, res_matrix);


    return 0;
}