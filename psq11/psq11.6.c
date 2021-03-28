/* Return a value to main(general case - dynamically allocate space)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{
    double myd = 3733.001;  
    double *Pmyd;
    Pmyd = (double *)malloc(sizeof(double));
    *Pmyd = myd;
    pthread_exit((void*)Pmyd);
}

void main() 
{ 
    pthread_t tid;
    double d;
    double ret;
    double *pRet;

    pthread_create(&tid, NULL, myth, (void *)&d);
    pthread_join(tid, (void**)&pRet);
    ret = *pRet;
    printf("%.3lf\n", ret);
    return;
}