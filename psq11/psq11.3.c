/* Pass a value to a thread(general case - pass the address of the variable)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{ 
    double myd; 
    myd = *((double *)arg);

    printf("%.3lf\n", myd); 
    pthread_exit(NULL);
}

void main() 
{ 
    pthread_t tid;
    double d = 3733.001;

    pthread_create(&tid, NULL, myth, (void *)&d);
    pthread_join(tid, NULL);
    return;
}