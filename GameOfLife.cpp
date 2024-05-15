#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int **allocate_array(int size);
void free_array(int **array, int size);
void initialize_random(int **array1, int **array2, int size);

int main() {
    int size, steps, num_threads;
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    printf("Enter the number of steps: ");
    scanf("%d", &steps);

    int **current, **previous, **temp;
    int t, i, j, neighbors;

    double time_taken;
    clock_t start, end;

    current = allocate_array(size);
    previous = allocate_array(size);
    initialize_random(previous, current, size);

    for (num_threads = 1; num_threads <= 8; num_threads++) {
        omp_set_num_threads(num_threads);
        printf("Number of threads = %d\n", num_threads);
        start = clock();

        #pragma omp parallel for private(i, j, neighbors)
        for (t = 0; t < steps; t++) {
            for (i = 1; i < size - 1; i++) {
                for (j = 1; j < size - 1; j++) {
                    neighbors = previous[i + 1][j + 1] + previous[i + 1][j] + previous[i + 1][j - 1] +
                               previous[i][j - 1] + previous[i][j + 1] +
                               previous[i - 1][j - 1] + previous[i - 1][j] + previous[i - 1][j + 1];

                    if (neighbors == 3 || (previous[i][j] + neighbors == 3))
                        current[i][j] = 1;
                    else
                        current[i][j] = 0;
                }
            }

            temp = current;
            current = previous;
            previous = temp;
        }

        end = clock();
        time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("GameOfLife: Size %d Steps %d Time %lf\n", size, steps, time_taken);
        printf("\n");
    }

    free_array(current, size);
    free_array(previous, size);
    return 0;
}

int **allocate_array(int size) {
    int **array;
    int i, j;
    array = (int **)malloc(size * sizeof(int *));
    for (i = 0; i < size; i++)
        array[i] = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            array[i][j] = 0;

    return array;
}

void free_array(int **array, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

void initialize_random(int **array1, int **array2, int size) {
    int i, position;
    srand(time(NULL));
    for (i = 0; i < (size * size) / 10; i++) {
        position = rand() % ((size - 2) * (size - 2));
        array1[position % (size - 2) + 1][(position / (size - 2)) + 1] = 1;
        array2[position % (size - 2) + 1][(position / (size - 2)) + 1] = 1;
    }
}