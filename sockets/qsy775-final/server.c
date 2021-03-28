/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define SIZE 50
#define port 8080

typedef struct 
{ 
    char ID[7]; 
    char name[10]; 
    double salary; 
}employee_info; 
 
 employee_info Array[10] = {{"fdf487", "Rogelio", 100000.0}, 
                               {"qsj234", "Joan", 990000.0}, //hangs
                               {"sfd251", "Mem", 142398.0}, 
                               {"oie973", "Juan", 645314.0}, 
                               {"tur588", "Harris", 142398.0}, 
                               {"jgf104", "Andrew", 789012.0}, 
                               {"zpq160", "Perez", 980123.0}, 
                               {"tqu724", "Wu", 524391.0}, 
                               {"qwe510", "Joe", 112980.0}, //hangs 
                               {"qew227", "Ronnie", 978423.0}}; 

int check(char *find, char *this) 
{
    int w = 0;
    int found = 0;
    for(w = 0; w < 5; w++) 
    { 
      if(this[w] != find[w]) 
      { 
        found = 1; 
        break;
      }
    }
    return found;
}

int findEmployeeID(int connectionSocket) 
{ 
    char buf[80]; 
    char *p; 
    int flg = 0; 
    int i;

    memset((char *)&buf, 0, sizeof(buf)); 
    recv(connectionSocket, buf, 80, 0); 
    p = strtok(buf, " "); 

    if(strcmp("STOP", p) == 0) 
    { 
        printf("[-] Closing Connection ...\n");
        //close(connectionSocket); 
        return -1;
    } 
    p = strtok(NULL, " "); 
    printf("Client Request: GETSALARY %s\n", p); 
    printf("%s\n", p);

    for(i = 0; i < 10; i++) 
    {
      if(check(Array[i].ID, p) == 0) 
      { 
        memset((char *)&buf, 0, sizeof(buf)); 
        sprintf(buf, "%s\n%.2f\n", Array[i].name, Array[i].salary);
        printf("\n%s\n", buf);
        send(connectionSocket, buf, strlen(buf), 0);
        flg = 1; 
        //return 1;
        break;
      }
    } 
    if(flg != 1) 
    { 
      //fprintf(stderr, "Employee not found\n");
      memset((char *)&buf, 0, sizeof(buf)); 
      strcpy(buf, "Employee not found\n"); 
      send(connectionSocket, buf, strlen(buf), 0); 
      exit(-1); 
    } 
    flg = 0;
    return 1;
}

int main()
{    
  struct  sockaddr_in sad; /* structure to hold server's address  */
  struct  sockaddr_in cad; /* structure to hold client's address  */
  int     alen;            /* length of address                   */
  
  int     welcomeSocket, connectionSocket; /* socket descriptors  */
  int     i, n;
  pid_t childPid;
  int check;
  int status;
  
  /* Create a socket */

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0); /* CREATE SOCKET */
  if (welcomeSocket < 0) {
    fprintf(stderr, "[-] Socket creation failed\n");
    exit(1);
  }
  
  /* Bind a local address to the socket */
  
  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_addr.s_addr = inet_addr("129.115.27.205");   /* set the local IP address   */
  sad.sin_port = htons((u_short)port);/* set the port number        */ 
  
  if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"bind failed\n");
    exit(1);
  } 
  
  /* Specify the size of request queue */
  
  if (listen(welcomeSocket, 10) < 0) {
    fprintf(stderr,"listen failed\n");
    exit(1);
  }
  printf("[+] Server is listening ...\n");
  
  /* Main server loop - accept and handle requests */ 
  int quickCheck = 1;
  while(quickCheck == 1) { 
    //WAIT:
    alen = sizeof(cad);
    if ( (connectionSocket=accept(welcomeSocket, (struct sockaddr *)&cad, &alen)) < 0) {
      fprintf(stderr, "accept failed\n");
      exit(-1);
    } 
    printf("Server accepted Client ...\n");
    childPid = fork(); 

    if(childPid < 0) 
    { 
        fprintf(stderr, "ERROR: Process Creation for Client Failed\n"); 
        exit(-1);
    }
    if(childPid == 0) 
    {
        //close(welcomeSocket); 
        if(findEmployeeID(connectionSocket) == -1) 
        { 
          quickCheck = 0;
          close(connectionSocket);
        }
        //fprintf(stderr, "[+] Server Connection Closed\n");
        //break;
    }
    else
    { 
        wait(&status);
        quickCheck = 0;
        //close(connectionSocket);
    } 
  }
}