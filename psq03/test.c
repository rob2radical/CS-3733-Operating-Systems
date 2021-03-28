#include <stdio.h>
#include <stdlib.h>

char *get_meName(void); 

int main() 
{ 
    char *buffer = get_meName();
    printf("%s", buffer);

    return 0;
}

char *get_meName(void) 
{ 
    char buff[100]; 

    buff = fgets(buff, 100, stdin);

    scanf("%s", buff);

    return buff;
}