#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 

void comm(int sockfd) 
{ 
    char buffer[MAX]; 
    int i; 

    for(;;) 
    { 
        bzero(buffer, MAX); 

        read(sockfd, buffer, sizeof(buffer)); 
        printf("From client: %s\t To client: ", buffer); 
        bzero(buffer, MAX); 
        i = 0; 
        
        while((buffer[i++] = getchar()) != '\n') 
        { 
            write(sockfd, buffer, sizeof(buffer)); 

            if(strncmp("exit", buffer, 4) == 0) 
            { 
                fprintf(stdout, "Server Exit...\n"); 
                break;
            }
        }
    }
} 

int main() 
{ 
    int sockfd; 
    int connfd; 
    int len; 

    struct sockaddr_in servaddr; 
    struct sockaddr_in cli; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    if(sockfd == -1) 
    { 
        fprintf(stderr, "ERROR: Socket creation failed\n"); 
        exit(-1);
    } 
    else
    {
        fprintf(stdout, "Socket was successfully created\n");
        bzero(&servaddr, sizeof(servaddr)); 

        servaddr.sin_family = AF_INET; 
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
        servaddr.sin_port = htons(PORT); 

        if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
        { 
            fprintf(stderr, "ERROR: Socket Bind Failed\n"); 
            exit(-1);
        } 
        else
        { 
            fprintf(stdout, "Socket bind was successful\n"); 

            if((listen(sockfd, 5)) != 0) 
            { 
                fprintf(stderr, "Socket Listen failed\n"); 
                exit(-1); 
            } 
            else
            {
                fprintf(stdout, "Server is listening...\n");  
                len = sizeof(cli); 

                connfd = accept(sockfd, (SA*)&cli, &len); 

                if(connfd < 0) 
                { 
                    fprintf(stdout, "Socket was successfully created\n"); 
                    exit(-1);
                } 
                else
                {
                    fprintf(stdout, "Server has accepted client...\n"); 
                } 
                comm(connfd); 

                close(sockfd);

            }
        }
    } 


}