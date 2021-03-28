#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>

void main() 
{ 
    pid_t proc = 0;
    printf("L0\n");
    fork();
    printf("L1\n");
    fork();
    printf("Bye\n");

    return;
}