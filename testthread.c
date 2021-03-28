#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5

int i;

void *PrintHello(void *threadid);
int main(int argc, char *argv[]){
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0;t<NUM_THREADS;t++){
         printf("In main: creating thread %ld\n", t);
         rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
         if (rc){
              printf("ERROR; return code from pthread_create() is %d\n", rc);
              exit(-1);
          }
    } // wait for other threads
    for(t=0;t<NUM_THREADS;t++)  pthread_join(threads[t], NULL);  
}
void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   for(i=0; i < 10; i++) printf("It's thread #%ld and i=%d\n", tid, i);
   pthread_exit(NULL);
}
