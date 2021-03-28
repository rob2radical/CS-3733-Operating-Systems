#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void) 
{ 
    int fd; 
    char buf[5] = "WXYZ";
    fork();
    fd = open("mydatafile.txt", 'r');
    read(fd, buf, 1);
    read(fd, buf+1, 1);
    printf("%c%c\n", buf[0], buf[1]);
    return 0;
}