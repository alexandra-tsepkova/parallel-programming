/******************************************************************************
 * ЗАДАНИЕ: bugged2.c
 * ОПИСАНИЕ:
 *   Еще одна программа на OpenMP с багом.
 *   Программа демонстрирует возможности динамического распределения нагрузки
 *   при вычислении суммы элементов арифметической прогрессии.
 ******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    // переменной типа float недостаточно, если мы хотим считать большие суммы, она переполняется
    double total;

// нужно, чтобы tid и i были приватными, потому что иначе, пока один тред работает с ними, другой может их перетереть своими значениями
// это может привести к некорректным результатам, например, у треда 1 в переменной tid будет написано 2

// для nthreads это не имеет значения, потому что ее использует только тред 0 один раз, можно ничего не менять

// total должна быть shared, так как в нее все треды будут складывать свою сумму
// так как она объявлена перед параллельной секцией, она и так shared, поэтому можно ничего не писать

#pragma omp parallel private(tid, i)
    {
        tid = omp_get_thread_num();
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d is starting...\n", tid);

        #pragma omp barrier

        total = 0.0;

// добавлена reduction, потому что без этого  программа будет считать неверно

        #pragma omp for schedule(dynamic, 10) reduction(+:total)
            for (i = 0; i < 1000000; i++){
               total += i*1.0;
         }

        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
}