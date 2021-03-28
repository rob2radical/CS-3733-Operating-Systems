#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double BALANCE = 0.0;
pthread_mutex_t lock;
typedef struct param { 
    int x;
    double y;
} param_t;

void *deposit(void *args) 
{ 
    int i; 
    param_t *ptr = (struct param *)args;

    for( i = 0; i < ptr->x; i++) 
    { 
        pthread_mutex_lock(&lock);
        BALANCE += ptr->y;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *withdraw(void *args) 
{ 
    int i;
    param_t *ptr = (param_t*)args;

    for(i = 0; i < ptr->x; i++) 
    { 
        pthread_mutex_lock(&lock);
        BALANCE -= ptr->y;
        pthread_mutex_unlock(&lock);
    } 
    return NULL;
}

int main(int argc, char *argv[]) 
{ 
    struct param dep, with;
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    dep.x = atoi(argv[1]);
    dep.y = atof(argv[2]);
    with.x = atoi(argv[3]);
    with.y = atof(argv[4]);

    pthread_create(&t1, NULL, deposit, &dep);
    pthread_create(&t2, NULL, withdraw, &with);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    double total = (dep.x * dep.y) - (with.x * with.y);
    printf("Balance = $%.2lf vs Total = $%.2lf\n", BALANCE, total);
    return 0;

}
/* 
qsy775@fox05:~/courses/cs3733/psq13$ qsy775-psq13 1000 3.5 500 2.8
Balance = $2100.00 vs Total = $2100.00
qsy775@fox05:~/courses/cs3733/psq13$

qsy775@fox05:~/courses/cs3733/psq13$ qsy775-psq13 10000 10.8 5000 5.7
Balance = $79500.00 vs Total = $79500.00
qsy775@fox05:~/courses/cs3733/psq13$

The balance is equal to a*b-c*d because if for example we deposit $3.50
1000 times that leaves us with $3,500 and then if we withdraw 1,400 that
leaves us with a balance of $2,100. In terms of a, b, c, and d we could
say (1000 * 3.50) - (500 * 2.80) = 2,100. 
*/


