#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>


#define READ_FLAGS O_RDONLY 
#define WRITE_FLAGS (O_WRONLY | O_CREAT | O_EXCL) 
#define WRITE_PERMS (S_IRUSR | S_IWUSR)
#define BLKSIZE 1024

int copyfile(int fromfd, int tofd) 
{ 
    char *bp; 
    char buf[BLKSIZE]; 
    int bytesread, byteswritten; 
    int totalbytes = 0; 
    
    for ( ; ; ) 
    { 
        while (((bytesread = read(fromfd, buf, BLKSIZE)) == -1) && 
        (errno == EINTR)); /* handle interruption by signal */ 
        
        if (bytesread <= 0) /* real error or end-of-file on fromfd */ 
        break; 
        bp = buf; 
        while (bytesread > 0) 
        { 
            while(((byteswritten = write(tofd, bp, bytesread)) == -1 ) && 
            (errno == EINTR)) ; /* handle interruption by signal */ 
            if (byteswritten <= 0) /* real error on tofd */ 
            break; 
            totalbytes += byteswritten; 
            bytesread -= byteswritten; 
            bp += byteswritten; 
        } 
        if (byteswritten == -1) /* real error on tofd */ 
        break; 
        } 
        return totalbytes; 
}

int main(int argc, char *argv[]) 
{ 
    int bytes;
    int fromfd, tofd;

    if(argc != 3) 
    { 
        fprintf(stderr, "Usage: %s from_file\n", argv[0]); 
        return 1;
    } 

    if((fromfd = open(argv[1], READ_FLAGS)) == -1) 
    { 
        perror("Failed to create output file\n");
        return 1;

    } 

    bytes = copyfile(fromfd, tofd); 
    printf("%d bytes copied from %s to %s\n", bytes, argv[1], argv[2]); 
    close(fromfd);
    close(tofd);
    return 0;

}