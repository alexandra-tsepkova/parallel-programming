#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int thread_count;
double a, b, h;
int n, local_n;
pthread_mutex_t mutex;
double total;

void *thread_job(void *rank);

double trap(double local_a, double local_b);


double f(double x) {
    return sin(1 / x);
}

int main(int argc, char **argv) {
    long i;
    pthread_t *threads;

    total = 0.0;
    thread_count = strtol(argv[1], NULL, 10);

    printf("Input a, b, h\n");
    scanf("%lf %lf %lf", &a, &b, &h);
    n = (b - a) / h;
    local_n = n / thread_count;

    threads = malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, thread_job,
                       (void *) i);
    }

    for (i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Integral from %f to %f = %8.15e with step %lf\n",
           a, b, total, h);

    pthread_mutex_destroy(&mutex);
    free(threads);

    return 0;
}

void *thread_job(void *rank) {
    double local_a;
    double local_b;
    double local_integral;
    long my_rank = (long) rank;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    local_integral = trap(local_a, local_b);

    pthread_mutex_lock(&mutex);
    total += local_integral;
    pthread_mutex_unlock(&mutex);
}

double trap(double local_a, double local_b) {
    double result;
    double x;
    int i;

    result = (f(local_a) + f(local_b)) / 2.0;
    for (i = 1; i <= local_n - 1; i++) {
        x = local_a + i * h;
        result += f(x);
    }
    result = result * h;
    return result;
}
