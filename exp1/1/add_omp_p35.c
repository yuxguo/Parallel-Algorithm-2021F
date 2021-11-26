#include <stdio.h>
#include <omp.h>

#define N (1 << 3)

int main(void)
{
    int array[N] = {1, 2, 3, 4, 5, 6, 7, 8};
    // omp_set_num_threads(8);
    // printf("%d\n", omp_get_num_threads());
    
    for (int i = 1; i < N; i <<= 1)
    {
#pragma omp parallel for
            for (int j = 0; j < N; j += (i << 1))
            {
                // printf("%d, %d\n", j, i+j);
                array[j] += array[j + i];
            }
        // printf("\n");
    }
    printf("After Computing.\n");
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", array[i]);
    }

    for (int i = N; i > 1; i >>= 1)
    {
#pragma omp parallel for
            for (int j = 0; j < N; j += i)
            {
                // printf("%d, %d\n", (i >> 1) + j, j);
                array[j + (i >> 1)] = array[j];
            }
            
        // printf("\n");
    }
    printf("\nAfter Broadcasting.\n");
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", array[i]);
    }

    return 0;
}
