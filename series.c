#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int world_size, commrank;
  int N, n_first_comm, n_other_comm;
  double local_sum = 0.0, global_sum;

  N = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &commrank);

  n_other_comm = (int)(N / world_size);
  n_first_comm = n_other_comm + N % world_size;

  if (commrank == 0) {
    int i = 1;
    while (i < world_size) {
      int n_start = n_first_comm + 1 + (i - 1) * n_other_comm;
      i++;
    }

    int n = 1;
    while (n <= n_first_comm) {
      local_sum += (1.0 / n);
      n++;
    }

  } else {
    int n = n_first_comm + 1 + (commrank - 1) * n_other_comm;
    int n_start = n;
    while (n < (n_start + n_other_comm)) {
      local_sum += (1.0 / n);
      n++;
    }
  }

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (commrank == 0) {
    printf("Sum of series 1/n for n from 1 to %d = %.4lf\n", N, global_sum);
  }

  MPI_Finalize();
  return 0;
}
