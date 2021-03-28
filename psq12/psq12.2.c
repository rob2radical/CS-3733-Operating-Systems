/* Pass an integer value and a double value to a thread(use STRUCT)*/
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
    param_t myT, *myPt;
    myT = *((param_t*)arg);
    myPt = (param_t*)arg;
    
    printf("%d\n", (myT.val));
    printf("%.3lf\n", myPt->db);
    pthread_exit(NULL);
}

void main() 
{ 
    pthread_t tid;
    int i = 3733;
    double d = 3733.001;
    param_t t_struct;
    param_t *p;
    p = malloc(sizeof(param_t));
    t_struct.val = i;
    t_struct.db = d;
    *p = t_struct;

    
    pthread_create(&tid, NULL, myth, (void*)&t_struct);
    pthread_join(tid, NULL);
    return;
}