/* Return an array of double values to main*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Pass an integer value and a double value to a thread (use struct!)
typedef struct param { 
    int val;
    double db;
}param_t;

void *myth(void *arg) 
{ 
    double myd[3] = {3.14, 2.5, 1.1}; 
    double *myDPtr;
    int i;
    myDPtr = (double *)malloc(3 * sizeof(double));
    for(i = 0; i < 3; i++) 
    { 
        myDPtr[i] = myd[i];
    }
    pthread_exit((void *)myDPtr);
}

void main() 
{ 
    pthread_t tid;
    double d[3];
    double *ret;
    int i;

    
    pthread_create(&tid, NULL, myth, (void*)d);
    pthread_join(tid, (void **)&ret);

    for(i = 0; i < 3; i++) 
    {
        printf("%.2lf\n", (double)ret[i]);
    }
    return;
}