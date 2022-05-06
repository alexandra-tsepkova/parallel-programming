#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double f(double t, double x) { return 0; }

double phi(double x) { return exp(-(x * x)); }

double psi(double t) { return exp(-(t * t)); }

void print_row(double *row, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%.4lf  ", row[i]);
    }
    printf("\n");
    return;
}

void print_row_to_file(double *row, int size, char * filename){
    FILE *f = fopen(filename, "w+");
    int i;
    for (i = 0; i < size; i++) {
        fprintf(f,"%.4lf  ", row[i]);
    }
    fprintf(f, "\n");
    return;
}

int main(int argc, char **argv) {
    int T = (argc >= 2) ? atoi(argv[1]) : 1;
    int X = (argc >= 3) ? atoi(argv[2]) : 2;
    int K = (argc >= 4) ? atoi(argv[3]) : 10;
    int M = (argc >= 5) ? atoi(argv[4]) : 10;

    double dx = (double) X / M;
    double dt = (double) T / K;
    double del = dt / dx;
    int k, m, i;

    double **u = (double **) malloc(sizeof(double *) * K);

    for (i = 0; i < K; i++) {
        u[i] = (double *) malloc(sizeof(double) * M);
    }

    int num_procs, rank;
    double start_wtime, finish_wtime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (k = 0; k < K; k++) {
        u[k][0] = psi(dt * k);
    }

    for (m = 0; m < M; m++) {
        u[0][m] = phi(dx * m);
    }

    int batch_num = (K - 1) / num_procs + (((K - 1) % num_procs == 0) ? 0 : 1);
    int last_batch_begin = ((K - 1) % num_procs == 0) ? (K - num_procs) : ((K - 1) / num_procs) * num_procs + 1;

    if (rank == 0) {
        start_wtime = MPI_Wtime();
        for (k = rank; k < (K - 1); k += num_procs) {
            if (k == 0) {
                for (m = 1; m < M; m++) {
                    u[k + 1][m] =
                            u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                    MPI_Send(&u[k + 1][m], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                }
            } else if (k + 1 >= last_batch_begin) {
                for (m = 1; m < M; m++) {
                    MPI_Recv(&u[k][m], 1, MPI_DOUBLE, num_procs - 1, MPI_ANY_TAG,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    u[k + 1][m] =
                            u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                    if (k + 1 != K - 1) {
                        MPI_Send(&u[k + 1][m], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                    }
                }

            } else {
                for (m = 1; m < M; m++) {
                    MPI_Recv(&u[k][m], 1, MPI_DOUBLE, num_procs - 1, MPI_ANY_TAG,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    u[k + 1][m] =
                            u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                    MPI_Send(&u[k + 1][m], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                }
            }
        }
    } else if (rank == num_procs - 1) {
        for (k = rank; k < (K - 2 + num_procs); k += num_procs) {
            if (k == rank) {
                for (m = 1; m < M; m++) {
                    MPI_Recv(&u[k][m], 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    u[k + 1][m] =
                            u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                }
            } else if (k + 1 >= last_batch_begin) {
                for (m = 1; m < M; m++) {
                    MPI_Send(&u[k + 1 - num_procs][m], 1, MPI_DOUBLE, 0, 0,
                             MPI_COMM_WORLD);
                    if (k + 1 == K - 1) {
                        MPI_Recv(&u[k][m], 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG,
                                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        u[k + 1][m] = u[k][m] - del * (u[k][m] - u[k][m - 1]) +
                                      dt * f(k * dt, m * dx);
                    }
                }
            } else {
                for (m = 1; m < M; m++) {
                    MPI_Send(&u[k + 1 - num_procs][m], 1, MPI_DOUBLE, 0, 0,
                             MPI_COMM_WORLD);
                    MPI_Recv(&u[k][m], 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    u[k + 1][m] =
                            u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                }
            }
        }

    } else {
        for (k = rank; k < (K - 1); k += num_procs) {
            for (m = 1; m < M; m++) {
                MPI_Recv(&u[k][m], 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                u[k + 1][m] =
                        u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(k * dt, m * dx);
                if (k != K - 2) {
                    MPI_Send(&u[k + 1][m], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
                }
            }

        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (k = 0; k + num_procs < K; k+=num_procs){
        MPI_Gather(u[1 + k + rank], M + (M % 2 == 0 ? 2 : 1), MPI_DOUBLE, u[1 + k + rank], M + (M % 2 == 0 ? 2 : 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    int len_last_batch = (K - 1) - last_batch_begin + 1;
    if ((len_last_batch > 1)  && (len_last_batch < num_procs)){

        double **tmp = (double **) malloc(sizeof(double *) * num_procs);

        for (i = 0; i < num_procs; i++) {
            tmp[i] = (double *) malloc(sizeof(double) * M);
        }

        for (k = 1; k < len_last_batch; k ++){
            if(rank == k){
                MPI_Send(&u[last_batch_begin + k][0], M, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
            else if (rank == 0){
                MPI_Recv(&u[last_batch_begin + k][0], M, MPI_DOUBLE, k, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    if (rank == 0) {
        for (k = 0; k < K; k++){
            print_row_to_file(u[k], M, "data_parallel.txt");
        }
    }
    MPI_Finalize();
    return 0;
}
