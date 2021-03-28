#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

int main(void) 
{ 
    int x; 
    x = 0; 
    if(fork() == 0) 
    { 
        x = 2; 
    } 
    else 
    { 
        exit(-1); 
    } 
    printf("I am process %ld and my x is %d\n", (long)getpid(), x);
}