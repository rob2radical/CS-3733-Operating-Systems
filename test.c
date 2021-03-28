#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 

void main() 
{ 
    int c1 = 0;
    int c2 = 0; 

    c1 = fork(); // create one child process
                    // 2^2 = 4
    c2 = fork(); // create one child process

    if(c1 == 0 && c2 == 0) 
    { 
        fork(); // create one child process
        printf("Child process made it here\n");
        exit(-1);
    }
    exit(-1);

}
