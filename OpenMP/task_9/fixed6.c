/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
*   Программа должнна была показывать, как можно выполнять параллельный код,
*   используя функции, но что-то пошло не так.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

// сделаем sum глобальной переменной
// так как при создании ее внутри функции dotprod мы делаем ее приватной и reduction не работает
// а при создании ее внутри main она не видна из dotprod, и мы не можем к ней суммировать внутри dotprod
// поэтому это выглядит как самое разумное решение, учитывая, что массивы a и b тоже глобальные
float sum;

// сделаем функцию dotprod void, потому что она ничего не возвращает и модифицирует глобальную переменную
void dotprod()
{
    int i, tid;

    tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
    for (i = 0; i < VECLEN; i++)
    {
        sum = sum + (a[i] * b[i]);
        printf("  tid= %d i=%d\n", tid, i);
    }
}


int main (int argc, char *argv[])
{
    int i;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;

#pragma omp parallel shared(sum)
    dotprod();

    printf("Sum = %f\n",sum);
}


