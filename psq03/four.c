#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

void main() 
{ 
    if(fork() == 0) 
    { 
        printf("Terminate\n"); 
        exit(0);
    }
    else
    { 
        printf("Running\n");
        while(1);

    }
}