#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main(int argc, char *argv[]) 
{ 
    int i;
    int n; 
    pid_t childpid = 0; 

    if(argc != 2) 
    { 
        fprintf(stderr, "Usage: %s [# of processes]\n", argv[0]);
    } 
    n = atoi(argv[1]); 
    
    for(i = 1; i < n; i++) 
    { 
        if((childpid = fork()) <= 0) 
        { 
            break; 
        } 
        fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n", i, (long)getpid(), (long)getppid(), (long)childpid); 
    } 
    return 0; 
}


