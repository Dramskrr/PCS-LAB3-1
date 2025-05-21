#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

typedef enum { false, true } bool;

const int ARRAY_SIZE = 2000000; 

void PrintArray(int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d ", array[i]);
    }
}

unsigned long int ComputeSum(bool threading, int thread_amount, int* array) {
    unsigned long int sum = 0;

    if (threading) { // Параллельно
        #pragma omp parallel num_threads(thread_amount)
        {   
            #pragma omp master
            {
                printf("Количество потоков: %d \n", omp_get_num_threads());
            }

            #pragma omp for reduction (+:sum)
            for (int i = 0; i < ARRAY_SIZE; ++i) {
                sum += array[i];
            }
        }
    } else { // Последовательно
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            sum += array[i];
        }
    }

    return sum;
}

void Task1(double* mean_time_sequential, double* mean_time_2_threads,
            double* mean_time_4_threads, double* mean_time_8_threads) {
    struct timespec begin, end;
    unsigned long int sum = 0;
    double taken_time = 0.0;

    printf("Количетсво элементов в массиве: %d \n", ARRAY_SIZE);

    srand(time(0));
    int array[ARRAY_SIZE]; // Создаём и заполняем массив
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 100;
    }

    //PrintArray(array); //для увеселения души

    // Последовательно
    clock_gettime(CLOCK_REALTIME, &begin);
    sum = ComputeSum(false, 0, array);
    clock_gettime(CLOCK_REALTIME, &end);
    taken_time = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1e9;
    printf("Последовательно: занятое время: %f секунд, результат: %.0f \n", taken_time, (double)sum);
    *mean_time_sequential += taken_time;

    // 2 потока
    clock_gettime(CLOCK_REALTIME, &begin);
    sum = ComputeSum(true, 2, array);
    clock_gettime(CLOCK_REALTIME, &end);
    taken_time = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1e9;
    printf("Параллельно 2 потока: занятое время: %f секунд, результат: %.0f \n", taken_time, (double)sum);
    *mean_time_2_threads += taken_time;

    // 4 потока
    clock_gettime(CLOCK_REALTIME, &begin);
    sum = ComputeSum(true, 4, array);
    clock_gettime(CLOCK_REALTIME, &end);
    taken_time = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1e9;
    printf("Параллельно 4 потока: занятое время: %f секунд, результат: %.0f \n", taken_time, (double)sum);
    *mean_time_4_threads += taken_time;

    // 8 потоков
    clock_gettime(CLOCK_REALTIME, &begin);
    sum = ComputeSum(true, 8, array);
    clock_gettime(CLOCK_REALTIME, &end);
    taken_time = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1e9;
    printf("Параллельно 8 потоков: занятое время: %f секунд, результат: %.0f \n\n", taken_time, (double)sum);
    *mean_time_8_threads += taken_time;

}

int main() {
    double mean_time_sequential = 0.0;
    double mean_time_2_threads = 0.0;
    double mean_time_4_threads = 0.0;
    double mean_time_8_threads = 0.0;
    int runs = 1000;
    
    for (int i = 0; i < runs; ++i) {
        
        Task1(&mean_time_sequential, &mean_time_2_threads, 
            &mean_time_4_threads, &mean_time_8_threads);
    }

    mean_time_sequential = mean_time_sequential / runs;
    mean_time_2_threads = mean_time_2_threads / runs;
    mean_time_4_threads = mean_time_4_threads / runs;
    mean_time_8_threads = mean_time_8_threads / runs;

    printf("Среднее время выполнения (%d выполнений):\nПоследовательно:%f\nПараллельно 2 потока:%f\nПараллельно 4 потока:%f\nПараллельно 8 потоков:%f\n",
            runs, mean_time_sequential, mean_time_2_threads, mean_time_4_threads, mean_time_8_threads);
    
}
