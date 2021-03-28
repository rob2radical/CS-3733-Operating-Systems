#include <stdio.h> 

int A[8]; 
int B[8];    
int i;  
int *ptr; 

int main(int argc, char *argv[]) 
{ 
    int C[8]; 
    int D[8] = {6}; 

    printf("argc    at %p contains %d \n", &argc, argc); 
    printf("argv    at %p contains %p\n", argv, *argv); 
    printf("argv[0] at %p contains %s\n", &argv[0], argv[0]); 
    printf("argv[%d] at %p contains %s\n", argc, &argv[argc] , argv); 

    printf("C: [0] at %p contains %d\n", &C[0], C[0]); 
    printf("C: [7] at %p contains %d\n", &C[7], C[7]); 
    printf("D: [0] at %p contains %d\n", &D[0], D[0]);
    printf("D: [7] at %p contains %d\n", &D[7], D[7]); 
    foo(5); 
    for(i = 0; i < 5; i++) 
    { 
        ptr = (int *) malloc(sizeof(int)); 
        printf("ptr     at %p points to %p\n", &ptr, ptr); 

    } 
    printf("A: [0] at %p contains %d\n", &A[0], A[0]); 
    printf("A: [7] at %p contains %d\n", &A[7], A[7]); 
    printf("B: [0] at %p contains %d\n", &B[0], B[0]); 
    printf("B: [7] at %p contains %d\n", &B[7], B[7]); 
    printf("foo    at %p\n", foo); 
    printf("main   at %p\n", main); 
    return 0;

} 

int foo(int x) 
{ 
    printf("foo     at %p x is at %p contains %d\n", foo, &x, x); 
    if (x > 0) foo(x - 1); 
    return x;
}