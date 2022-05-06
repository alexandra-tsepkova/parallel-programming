#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(int m, int k, double dt, double dx) {
    return 0;
}

double phi(int m, double dx) {
    double x = dx * m;
    return exp(-(x * x));
}

double psi(int k, double dt) {
    double t = dt * k;
    return exp(-(t * t));
}


int main(int argc, char **argv) {
    FILE *file = fopen("data_serial.txt", "w");
  int T = (argc >= 2) ? atoi(argv[1]) : 1;
  int X = (argc >= 3) ? atoi(argv[2]) : 2;
  int K = (argc >= 4) ? atoi(argv[3]) : 10;
  int M = (argc >= 5) ? atoi(argv[4]) : 9;

  double dx = (double)X / M;
  double dt = (double)T / K;
  double del = dt / dx;

  double **u = (double **)malloc(sizeof(double *) * K);

  for (int i = 0; i < K; i++) {
    u[i] = (double *)malloc(sizeof(double) * M);
  }

  int k, m;
  for (m = 0; m < M; m++) {
    u[0][m] = phi(m, dx);
  }
  for (k = 0; k < K; k++) {
      u[k][0] = psi(k, dt);
  }

  for (k = 0; k < (K - 1); k++) {
    for (m = 1; m < M; m++) {
      u[k + 1][m] =
          u[k][m] - del * (u[k][m] - u[k][m - 1]) + dt * f(m, k, dt, dx);
    }
  }


  for (k = 0; k < K; k++) {
    for (m = 0; m < M; m++) {
      fprintf(file,"%.4lf\t\t", u[k][m]);
    }
    fprintf(file,"\n");
  }
  return 0;
}