#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h> 

int main(void) 
{ 
    pid_t childpid; 
    int x = 1;

    childpid = fork(); 

    if(childpid == -1) 
    { 
        perror("Failed to fork\n"); 
        return 1; 
    } 
    if(childpid == 0) // child code
    { 
        x += 1;
        printf("I am child %ld and my x is %d\n", (long)getpid(), x);
    } 
    else // parent code 
    { 
        x -= 1; 
        printf("I am parent %ld and my x is %d\n", (long)getpid(), x--);
    }
    return 0;
}