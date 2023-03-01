#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 1000000000
#define THREAD_NUM 16

long long sum[THREAD_NUM];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *sum_func(void *arg) {
    int tid = *(int *)arg;
    long long local_sum = 0;

    for (int i = tid * (N / THREAD_NUM) + 1; i <= (tid + 1) * (N / THREAD_NUM); i++) {
        local_sum += i;
    }

    pthread_mutex_lock(&mutex);
    sum[tid] = local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    struct timeval start, end;
    long long total_sum = 0;
    pthread_t threads[THREAD_NUM];
    int tids[THREAD_NUM];

    gettimeofday(&start, NULL);

    for (int i = 0; i < THREAD_NUM; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, sum_func, (void *)&tids[i]);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
        total_sum += sum[i];
    }

    gettimeofday(&end, NULL);

    printf("Total sum: %lld\n", total_sum);
    printf("Time used: %ld ms\n", (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000);

    return 0;
}

