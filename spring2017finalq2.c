#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 

int main() 
{ 
    int fd; 
    char buf[6] = "12345"; 
    fd = open("tmpdata.txt", "r"); 
    fork(); 
    read(fd, buf, 2); 
    read(fd, buf+2, 2); 
    printf("%ld: %s ", (long)getpid(), buf); 

    return;
}
