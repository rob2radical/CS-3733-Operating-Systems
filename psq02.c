#include <stdio.h> 
#include <stdlib.h> 

int myFunc(int E); 

int A, B = 6; 

int main(int argc, char *argv[]) 
{ 
    int C, D; 
    A = 4; 
    C = myFunc(5); 
    D = myFunc(7); 
    printf("A = %d  B = %d  C = %d  D = %d\n", A, B, C, D); 
    return 0;
} 

int myFunc(int E) 
{ 
    int F = 0; 
    F = F + E; 
    return F;
}