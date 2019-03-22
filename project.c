#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
int T, E, N, M;

#define SEM_FLAG_NAME "/ten9"
#define SEM_FLAG_NAME2 "/aaaa"

int temp = 0;

sem_t *learn;
sem_t *nextLoop;

void *learningProcess(void *param)
{
    int tid = (int)param;
    temp++;

    sem_wait(learn);
    printf("[ %d ] IN\n", tid);
    printf("[ %d ] : Here is learning process\n", tid);

    sleep(10);

    printf("[ %d ] OUT\n", tid);
    sem_post(learn);
    if (temp == N)
    {
        temp = 0;
        sem_post(nextLoop);
    }

    return NULL;
}

int main(void)
{
    // =======For manual variable setting=========
    int numberOfThread = 6;
    int numberOfSimulator = 3;
    int simulatorPeriod = 3;
    int totalPeriod = 1000;
    // ===========================================

    N = numberOfThread;
    M = numberOfSimulator;
    T = totalPeriod;
    E = simulatorPeriod;
    printf("[ Variable Set ]\n");
    printf("[ STARTING ]\n");
    printf("[ %d Tachikoma Robots]\n", N);
    printf("[ %d Simulator]\n", M);

    pthread_t t[N];
    learn = sem_open(SEM_FLAG_NAME, O_CREAT, 0666, M);
    nextLoop = sem_open(SEM_FLAG_NAME2, O_CREAT, 0666, 1);

    while (1)
    {

        int i = 0;
        while (i < N)
        {
            pthread_create(&t[i], NULL, learningProcess, (void *)i);
            i++;
        }
        if (i == N)
        {
            // printf("Im in");
            sem_wait(nextLoop);
            // printf("============================\n");
        }
    }

    int i;
    for (i = 0; i < N; i++)
    {
        pthread_join(t[i], NULL);
    }
    sem_destroy(&learn);
    sem_destroy(&nextLoop);
    printf("[ ENDING ]\n");

    return 0;
}

// For User input
// int numberOfThread;
// int numberOfSimulator;
// int simulatorPeriod;
// int learningPeriod;
// printf("Enter the amount of Tachikoma robots :");
// scanf("%d", &numberOfThread);
// printf("Enter the amount of Simulator :");
// scanf("%d", &numberOfSimulator);
// printf("Enter the maximum period of time that a simulator runs in one playing (in seconds) :");
// scanf("%d", &simulatorPeriod);
// printf("Enter the fixed period of time that the whole learning will take :");
// scanf("%d", &totalPeriod);