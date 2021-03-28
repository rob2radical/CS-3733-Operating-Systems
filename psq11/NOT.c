#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *return_long_thread(void *arg) 
{
    long n1 = 0;
    static long n2 = 0;
    long *p1 = (long*)malloc(sizeof(long));
    if(p1 == NULL) 
    { 
        exit(-1);
    }
    return((void*)&n2);
}

void main() 
{ 
    pthread_t tid;
    long i;
    long *pi;

    pthread_create(&tid, NULL, return_long_thread, (void *)i);
    pthread_join(tid, (void**)&pi);
    i = (long)pi;
    printf("%ld\n", i);
    return;
}