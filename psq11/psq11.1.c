
/* Pass a value to a thread (special case - pass the value of value as a pointer value)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{
    long myi; 
    myi = (long)arg;

    printf("%ld\n", myi);

    pthread_exit(NULL);
}

int main() 
{ 
    pthread_t tid;
    long i = 3733; 
    
    pthread_create(&tid, NULL, myth, (void *)i);
    pthread_join(tid, NULL);
    printf("After thread\n");
    return 0; 
}