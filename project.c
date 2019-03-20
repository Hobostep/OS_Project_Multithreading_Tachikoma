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

    int numberOfThread = 5;
    int numberOfSimulator = 1;
    int simulatorPeriod = 10;
    int learningPeriod = 10;

    initialize(numberOfThread, numberOfSimulator, simulatorPeriod, learningPeriod);

    return 0;
}

// #include <stdio.h>
// #include <pthread.h>

// void *hello(void *input)
// {
//     printf("%s\n", (char *)input);
//     pthread_exit(NULL);
// }

// int main(void)
// {
//     pthread_t tid;
//     pthread_create(&tid, NULL, hello, "Test printing");
//     pthread_join(tid, NULL);
//     return 0;
// }