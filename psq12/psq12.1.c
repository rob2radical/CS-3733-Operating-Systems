/* Pass an array of values to a thread*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myth(void *arg) 
{ 
    double *myarr;
    myarr = (double *)arg;
    int i;
    //int length = sizeof(myarr)/sizeof(myarr[]);

    for(i = 0; i < sizeof(myarr); i++) 
    { 
        printf("%.2lf\n", myarr[i]);
    }
    pthread_exit(NULL);
}

void main() 
{ 
    pthread_t tid;
    double d[3] = {3.14, 2.5, 1.1};

    pthread_create(&tid, NULL, myth, (void *)d);
    pthread_join(tid,NULL);
    return;
}
