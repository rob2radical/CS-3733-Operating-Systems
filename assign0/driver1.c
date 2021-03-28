/*
 * File: driver1.c
 * Robert Quintanilla qsy775
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include "myio.h"
#define buffer 100

/*
 * First one (say driver1.c) gets three command-line arguments: x y z.  
 * It then asks user to enter x many integers, y many doubles, and z many lines. 
 * Prompt for each input at a time. 
 *
 * Everytime the program prints back the entered data on the stdout 
 *      while printing error messages on stderr (if any). 
 *
 * It also keeps track of the largest integer, largest double, and the longest string. 
 * At the end, your program (driver1.c) prints the largest integer, the largest double, 
 * and the longest line (longest string that were entered.) 
 */

int main(int argc, char *argv[])
{
  // YOU NEED TO IMPLEMENT THIS (USE FUNCTIONS FROM myio.h) 
  // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE
  int x, y, z;

  if(argc != 4) 
  {
    fprintf(stderr, "Error: Incorrect input, please provide [file_name] [x] [y] [z]\n");
    printf("x is number of integers, y is number of doubles, and z is number of lines\n");
    exit(-1);
  } 
  x = atoi(argv[1]); 
  y = atoi(argv[2]);
  z = atoi(argv[3]); 
  
  char *longestLine = calloc(buffer, sizeof(char)); 
  int largestInt = ReadInteger(x); 
  double largestDouble = ReadDouble(y); 
  longestLine = LongestLine(z); 

  printf("\nThe largest integer is %d", largestInt); 
  printf("\nThe largest double is %.2lf", largestDouble); 
  printf("\nThe longest line is: %s\n", longestLine);

  free(longestLine); 
  return 0; 
} 
   
