/* Return a value to main(general case - dynamically allocate space)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{
    long myi = 3733;  
    long *myPi = malloc(sizeof(long));
    *myPi = myi;
    pthread_exit((void*)myPi);
}

void main() 
{ 
    pthread_t tid;
    long i;
    long ret;
    long *pRet;

    pthread_create(&tid, NULL, myth, (void *)&i);
    pthread_join(tid, (void**)&pRet);
    ret = *pRet;
    printf("%ld\n", ret);
    return;
}