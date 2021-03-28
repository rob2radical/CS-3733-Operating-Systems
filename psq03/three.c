#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 

void main() 
{ 
    if(fork() == 0) 
    { 
        printf("a\n"); 
    } 
    else 
    { 
        printf("b\n"); 
        waitpid(-1, NULL, 0); // wait for any child process
    }
    printf("c\n"); 
    exit(0); 
}