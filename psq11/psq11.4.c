/* Return a value to main(special case - return the value of as a pointer)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{
    long myi = 3733;
    long myret;
    myret = myi;
    pthread_exit((void*)myret);
}

void main() 
{ 
    pthread_t tid;
    long i;
    long *pi;

    pthread_create(&tid, NULL, myth, (void *)i);
    pthread_join(tid, (void**)&pi);
    i = (long)pi;
    printf("%ld\n", i);
    return;
}