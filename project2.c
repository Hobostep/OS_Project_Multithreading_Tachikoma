#include <sys/time.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

struct timespec start;
int N, M, E, T, nReport = 1;
sem_t *learn;
int *learning, *report, *tottime;

uint64_t currtime()
{
    struct timespec curr;
    clock_gettime(CLOCK_MONOTONIC_RAW, &curr);
    return (curr.tv_sec - start.tv_sec) * 1000000 + (curr.tv_nsec - start.tv_nsec) / 1000;
}

int checkmin(int id)
{
    int i, thistot = tottime[id], check = 0;
    int num2check;
    if (M > N / 2)
    {
        num2check = N / 2;
    }
    else
    {
        num2check = M;
    }
    for (i = 0; i < N; i++)
    {
        if (thistot > tottime[i])
        {
            check++;
             
        }
        if (check > num2check)
        {
            break;
        }
    }

    if (check > num2check)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void *Learn(void *tid)
{
    int id = *(int *)tid;
    int bid, nid;
    int *check = (int *)tid;
    int time;

    if (id == 0)
    {
        bid = N - 1;
    }
    else
    {
        bid = id - 1;
    }

    if (id == N - 1)
    {
        nid = 0;
    }
    else
    {
        nid = id + 1;
    }

    while (*check != -1)
    {
        sem_wait(learn);
        if (learning[bid] == 0 && learning[nid] == 0)
        {
            time = rand() % E + 1;
            if ((currtime() / 1000000 + time) > T)
            {
                sem_post(learn);
                break;
            }
            printf("LEARN[%" PRIu64 "]: %d, %d, %d\n", currtime(), id, bid, nid);
            learning[id] = 1;
            sleep(time);
            printf("DONE[%" PRIu64 "]: %d, %d, %d\n", currtime(), id, bid, nid);
            sem_post(learn);
            tottime[id] += time;
            report[id]++;
            printf("UPDATE[%" PRIu64 "]: %d, %d\n", currtime(), id, nReport++);
            learning[id] = 0;
        }
        else
        {
            sem_post(learn);
            continue;
        }

        while (checkmin(id) && *check != -1)
        {
            sleep(1);
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int i, totreport = 0;
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    E = atoi(argv[3]);
    T = atoi(argv[4]);
    learning = malloc(N * sizeof(int));
    report = malloc(N * sizeof(int));
    tottime = malloc(N * sizeof(int));

    learn = sem_open("/maxlearn", O_CREAT, 0666, M);
    sem_unlink("/maxlearn");

    pthread_t threads[N];
    int tachId[N];
    srand(42);
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < N; i++)
    {
        tachId[i] = i;
        learning[i] = 0;
        report[i] = 0;
        tottime[i] = 0;
    }

    for (i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, Learn, (void *)&tachId[i]);
    }

    sleep(T);

    for (i = 0; i < N; i++)
    {
        tachId[i] = -1;
    }

    for (i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < N; i++)
    {
        printf("%d: %d, %d\n", i, tottime[i], report[i]);
        totreport += report[i];
    }

    printf("MASTER: %d\n", totreport);
    sem_close(learn);
    return 0;
}
