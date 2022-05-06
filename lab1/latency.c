#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int commsize, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  if (commsize < 2){
      printf("At least two processes are required!\n");
      MPI_Finalize();
      return 0;
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char message = 'a';
  MPI_Status status;
  if (rank == 0) {
    double start_wtime = MPI_Wtime();
    MPI_Recv(&message, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
    double finish_wtime = MPI_Wtime();

    printf("Latency: %lg seconds\n", finish_wtime - start_wtime);
  } else {
    MPI_Send(&message, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}