/* Return an int and double value to main(use STRUCT)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Return an integer value and a double value to main (use struct!)
typedef struct param { 
    int val;
    double db;
}param_t;

void *myth(void *arg) 
{ 
    int myi = 3733;
    double myd = 3733.001;
    param_t *p = (param_t*)malloc(sizeof(param_t));
    p->val = myi;
    p->db = myd; 
    pthread_exit((void*)p);
}

void main() 
{ 
    pthread_t tid;
    int i;
    double d;
    param_t *Pret;
    
    pthread_create(&tid, NULL, myth, (void*)Pret);
    pthread_join(tid, (void *)&Pret);
    printf("%d\n", Pret->val);
    printf("%.3lf\n", Pret->db);
    return;
}