#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void *learningProcess(void *param)
{
    printf("[ %d ] : Here is learning process\n", param);
    pthread_exit(NULL);
}

void initialize(N, M, E, T)
{
    int i;
    pthread_t t[N];
    // float arrayId[N];

    int indexes[N];

    for (i = 0; i < N; i++)
    {
        indexes[i] = i;
        pthread_create(&t[i], NULL, learningProcess, i);
        pthread_join(t[i], NULL);
    }
}

int main(void)
{

    int numberOfThread;
    int numberOfSimulator;
    int simulatorPeriod;
    int learningPeriod;

    // int numberOfThread = 5;
    // int numberOfSimulator = 1;
    // int simulatorPeriod = 10;
    // int learningPeriod = 10;

    // For User input
    printf("Enter the amount of Tachikoma robots :");
    scanf("%d", &numberOfThread);
    printf("Enter the amount of Simulator :");
    scanf("%d", &numberOfSimulator);
    printf("Enter the maximum period of time that a simulator runs in one playing (in seconds) :");
    scanf("%d", &simulatorPeriod);
    printf("Enter the fixed period of time that the whole learning will take :");
    scanf("%d", &learningPeriod);

    initialize(numberOfThread, numberOfSimulator, simulatorPeriod, learningPeriod);

    return 0;
}