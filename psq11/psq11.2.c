/* Pass a value to a thread(general case - pass the address of the variable)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{
    long myi; 
    myi = *((long *)arg);

    printf("%ld\n", myi); 
    pthread_exit(NULL);
}

void main() 
{ 
    pthread_t tid;
    long i = 3733;

    pthread_create(&tid, NULL, myth, (void *)&i);
    pthread_join(tid, NULL);
    return;
}