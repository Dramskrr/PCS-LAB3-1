#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

const int DEFAULT_ARRAY_SIZE = 1000000;
const int DEFAULT_RUNS = 100;
const int DEFAULT_CORES = 1;

int* CreateArray(const int SIZE) {
    int* int_array = (int*) malloc(sizeof(int) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        int_array[i] = rand();
    }
    return int_array;
}

void PrintArray(const int* array, const int SIZE) {
    for (int i = 0; i < SIZE; i++) {
        printf("%d ",array[i]);
    }
}

int GetEnvArraySize() {
    char* array_size_char = getenv("ARRAY_SIZE");
    int array_size_int = DEFAULT_ARRAY_SIZE;
    if (array_size_char != NULL) {
        array_size_int = atoi(array_size_char);
    } else {
        printf(
            "Переменная среды ARRAY_SIZE не получена, "
            "используем значение по умолчанию: %d \n", DEFAULT_ARRAY_SIZE
        );
    }
    return array_size_int;
}

int GetEnvCores() {
    char* cores_char = getenv("CORES");
    int cores_int = DEFAULT_CORES;
    if (cores_char != NULL) {
        cores_int = atoi(cores_char);
    } else {
        printf(
            "Переменная среды CORES не получена, "
            "используем значение по умолчанию: %d \n", DEFAULT_CORES
        );
    }
    return cores_int;
}

int GetEnvRuns() {
    char* runs_char = getenv("RUNS");
    int runs_int = DEFAULT_RUNS;
    if (runs_char != NULL) {
        runs_int = atoi(runs_char);
    } else {
        printf(
            "Переменная среды RUNS не получена, "
            "используем значение по умолчанию: %d \n", DEFAULT_RUNS
        );
    }
    return runs_int;
}

int64_t SumElementsOfArray(const int* array, const int SIZE,
                            const bool PARALLEL_MDOE) {
    if (!PARALLEL_MDOE) {
        int64_t result = 0;
        for (int i = 0; i < SIZE; i++) {
            result += array[0];
        }
        return result;
    }


}

int main(int argc, char** argv) {
    srand(time(NULL));
    const int ARRAY_SIZE = GetEnvArraySize();
    const int RUNS = GetEnvRuns();
    const int CORES = GetEnvCores();
    bool parallel_mode = false; 

    if (CORES > 1) {
        parallel_mode = true;
        printf("Программа отработает в параллельном "
                "режиме (используемых ядер > 1)\n"
        );
    } else {
        printf("Программа отработает в последовательном "
                "режиме (используемых ядер = 1)\n"
        );
    }

    printf("Размер массива: %d \n", ARRAY_SIZE);
    printf("Повторений: %d \n", RUNS);
    printf("Используемые ядра: %d \n", CORES);
    
    // Таймер
    struct timespec begin, end;
    double exec_time = 0.0;
    double mean_exec_time = 0.0;

    if (parallel_mode) {
        MPI_Init(&argc, &argv);
        MPI_Finalize();
    }

    printf("Рассчёты начаты...\n");
    for (int i = 0; i < RUNS; i++) {
        clock_gettime(CLOCK_REALTIME, &begin);
        int* int_array = CreateArray(ARRAY_SIZE);
        //PrintArray(int_array, ARRAY_SIZE);
        int64_t sum_result = SumElementsOfArray(int_array, ARRAY_SIZE, parallel_mode);
        //printf("Результат: %ld \n", sum_result);
        free(int_array);
        clock_gettime(CLOCK_REALTIME, &end);
        exec_time += (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1e9;
    }

    mean_exec_time = exec_time / RUNS;
    printf("Среднее время выполнения: %f сек.", mean_exec_time);

    return 0;
}
