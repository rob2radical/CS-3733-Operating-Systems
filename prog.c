#include<stdio.h>
#include<stdlib.h>

typedef struct point { 
    int x;
    int y;
} pointT;

typedef struct triangle { 
    pointT corner[3];
    char color;
    char *name;
    struct triangle *next;
} triangleT;

int main() 
{ 
    triangleT p, *q;
    p.corner[2].x = 6;
    p.name = "Equilateral";

    q = malloc(sizeof(triangleT));
    q->corner->y = 6;

    q->next = malloc(sizeof(triangleT));
    q->next->color = 'r';


}