#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

int main() 
{ 
    int x; 
    x = 0;
    char *args[] = {"./twoprocs.c", NULL};
    execvp(args[0], args); 
    x = 1; 
    printf("I am process %ld and my x is %d\n", (long)getpid(), x); 
    return 0;
}