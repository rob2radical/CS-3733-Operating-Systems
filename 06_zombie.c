#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

void main() 
{ 
    if(fork() == 0) 
    { 
        printf("Terminating child, PID = %d\n", getpid()); 
        
        while(1) ;
    } 
    else 
    { 
        printf("Running parent, PID = %d\n", getpid()); 
        exit(0); 
    } 
}