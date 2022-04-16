#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int commsize, my_rank, source, dest;
  int message = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Status status;

  if (my_rank == 0) {
    MPI_Send(&message, 1, MPI_INT, my_rank + 1, my_rank, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT, commsize - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
  } else if (my_rank == commsize - 1) {
    MPI_Recv(&message, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
    message++;
    MPI_Send(&message, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&message, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
    message++;
    MPI_Send(&message, 1, MPI_INT, my_rank + 1, my_rank, MPI_COMM_WORLD);
  }

  printf("My rank = %d, My message = %d\n", my_rank, message);
  MPI_Finalize();
  return 0;
}