
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void *func();

int main()
{
    int temp = 0;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, func, 0);
    pthread_join(tid, NULL);
    printf("End of this test\n");
}
void *func()
{
    printf("HELLO THIS IS THREAD FUNCTION\n");
    // pthread.exit(0);
}
