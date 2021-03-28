#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 

void comm(int sockfd) 
{ 
    char buffer[MAX]; 
    int i; 

    for(;;) 
    { 
        bzero(buffer, sizeof(buffer)); 
        printf("Enter a string: "); 
        i = 0; 

        while((buffer[i++] = getchar()) != '\n') 
        { 
            write(sockfd, buffer, sizeof(buffer)); 
            bzero(buffer, sizeof(buffer)); 
            read(sockfd, buffer, sizeof(buffer)); 
            printf("From Server: %s", buffer); 

            if((strncmp(buffer, "exit", 4)) == 0) 
            { 
                printf("Client Exit...\n"); 
                break;
            }
        }
    }
} 

int main() 
{ 
    int sockfd; 
    int connfd; 
    struct sockaddr_in servaddr; 
    struct sockaddr_in cli; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    if(sockfd == -1) 
    { 
        fprintf(stderr, "ERROR: Socket Creation Failed\n"); 
        exit(-1);    
    } 
    else
    {
        fprintf(stdout, "Socket was successfully created\n"); 
        bzero(&servaddr, sizeof(servaddr)); 

        servaddr.sin_family = AF_INET; 
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        servaddr.sin_port = htons(PORT); 

        if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
        { 
            fprintf(stderr, "Attempt to connect to server failed...\n"); 
            exit(-1);
        } 
        else 
        { 
            fprintf(stdout, "Socket was successfully created\n"); 
            comm(sockfd); 
            close(sockfd);
        }
    }
}