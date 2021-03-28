/*
 * File: myio.c
 * Robert Quintanilla 
 * YOU NEED TO IMPLEMENT THE FUNCTIONS  here
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "myio.h"
#define buffer 100
 
/*
 * Function: ReadInteger
 * Usage: i = ReadInteger();
 * ------------------------
 * ReadInteger reads a line of text from standard input and scans
 * it as an integer. To read the line, you can call the ReadLine()
 * function that is described below. If all the characters in the
 * given line are digits, then convert it to an integer and 
 * return it. The very first character could be the sign character
 * '-' or '+'. 
 * Otherwise, if the given string contains any other character, 
 * this function will give an error meassage and keep asking user
 * to enter a valid sequnece of characters that can be converted to
 * an integer. 
 * The followings are valid integers: 234, -345, +453.
 * The followings are not valid integers: 34  5, 3afg, 3-3, --45.   
 */
int ReadInteger(int userInts) 
{ 
  int j = 0; 
  char *userInput = calloc(buffer, sizeof(char)); 
  int *intArray = calloc(buffer, sizeof(char)); 
  int i;
  int maxInt;
  
  while(j < userInts) 
  { 
    printf("Enter an integer: "); 
    userInput = ReadLine();
    
    if((userInput[0] == '+' || userInput[0] == '-' || isdigit(userInput[0]))) 
    { 
      int inputLength = strlen(userInput); 
      int isDigit = 0; 
      
      for(i = 1; i < inputLength; i++) 
      { 
        if (isdigit(userInput[j]) == 0) 
        { 
          isDigit++; 
        } 
      } 
      if(isDigit == 0) 
      { 
        int integers = atoi(userInput);
        intArray[j] = integers;
        printf("%d\n", intArray[j]); 
        j++; 
      } 
      else 
      { 
        fprintf(stderr, "ERROR: The given input is incorrect. Please enter only integers.\n"); 
      } 
    } 
    /*else 
    { 
      fprintf(stderr, "%s", "ERROR: The input is incorrect. Please enter only integers.\n"); 
    }*/ 
    maxInt = findMaxInt(intArray, &userInts);
  } 
  free(userInput); 

  return maxInt;
} 

int findMaxInt(int *userIntArray, int *userNums) 
{ 
  int i;
  int size = *userNums;
  int maxInt = userIntArray[0]; 

  for(i = 0; i < size; i++) 
  { 
    if(userIntArray[i] > maxInt) 
    { 
      maxInt = userIntArray[i];
    }
  } 
  return maxInt;
}




/*
 * Function: ReadDouble
 * Usage: x = ReadDouble();
 * ---------------------
 * ReadDouble reads a line of text from standard input and scans
 * it as a double.  As described above, you can call ReadLine() to
 * read the line. If the number cannot be scanned as double or if
 * extra characters follow after the number ends, this function 
 * gives an error and keep asking user to enter a valid double. 
 * The followings are valid doubless: 24, -35, +43, -23.54, +45.3 
 * The followings are not valid doubles: 34  5, 3afg, 3.3.3, --45.5   
 */
double ReadDouble(int userDoubles) 
{ 
  int j = 0; 
  char *inputArr = calloc(buffer, sizeof(char)); 
  double doubleArr[100]; 
  int i; 
  int decimal = 0; 
  char *ptr;
  double maxDouble; 
  
  while(j < userDoubles) 
  { 
    printf("Enter a double: "); 
    inputArr = ReadLine(); 
    
    if((inputArr[0] == '+' || inputArr[0] == '-' || isdigit(inputArr[0]))) 
    { 
      int length = strlen(inputArr); 
      int cnt = 0; 
      for (i = 1; i < length; i++) 
      { 
        if(isdigit(inputArr[i]) == 0) 
        { 
          cnt++; 
        } 
        if(inputArr[i] == '.') 
        { 
          decimal++; 
          cnt =0; 
        } 
      } 
      if(cnt == 0 && decimal <= 1) 
      { 
        double doubles = strtod(inputArr,&ptr); 
        doubleArr[j] = doubles; 
        printf("%.2lf\n", doubleArr[j]); 
        j++; 
        decimal = 0; 
        } 
        else 
        { 
          fprintf(stderr, "%s", "ERROR: The input is incorrect. Please try again.\n"); 
          decimal = 0; 
        } 
    }
    maxDouble = findMaxDouble(doubleArr, &userDoubles);
  } 
  free(inputArr);
  return maxDouble;
} 

double findMaxDouble(double *userDoubleArray, int *userDoubles) 
{ 
  int i;
  int size = *userDoubles;
  double maxDouble = userDoubleArray[0]; 

  for(i = 0; i < size; i++) 
  { 
    if(userDoubleArray[i] > maxDouble) 
    { 
      maxDouble = userDoubleArray[i];
    }
  } 
  return maxDouble;
}


/* 
 * Function: LongestLine 
 * Usage: maxLine = LongestLine(z); 
 * --------------------------------
 * LongestLine keeps track of the longest line that is read
 * from stdin. It sets the first read line to max and compares 
 * the following read in strings length and sets max equal to
 * the value of the string that is 
 */ 
 char *LongestLine(int numLines) 
 { 
   int i;
   char *newInput = calloc(buffer, sizeof(char)); 
   char *userInput = calloc(buffer, sizeof(char));
   int longest = 0;

   for(i = 0; i < numLines; i++) 
   { 
     printf("Enter a string: "); 
     userInput = ReadLine();

     if(userInput != '\0') 
     { 
       int length = strlen(userInput); 

       if(length > longest ) 
       { 
         longest = length; 
         newInput = userInput; 

       }
     }
   } 
   return newInput;

 }



/*
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input stdin and returns
 * the line as a string.  The newline character that terminates
 * the input is not stored as part of the string.
 * 
 * Note: this function above ReadLine(); can simply be implemented 
 *       by using the next function as follows.. 
 *       So, focus on the implementation of ReadLineFile(infile); below  
 */
char *ReadLine(void)
{  
   return(ReadLineFile(stdin)); 
} 



/* 
 * Function: ReadLineFile
 * Usage: s = ReadLineFile(infile);
 * ----------------------------
 * ReadLineFile reads a line of text from the input file which 
 * is already open and pointed by infile. It then reads the line, 
 * dynamically allocates space, and returns the line as a string. 
 * The newline character that terminates the input is not stored 
 * as part of the string.  
 * The ReadLineFile function returns NULL if infile is at the 
 * end-of-file position. 
 */
char *ReadLineFile(FILE *infile) 
{ 
  char *userInput = calloc(buffer, sizeof(char));
  //userInput = (char *) malloc(buffer);
  int i; 

  if(fgets(userInput, buffer, infile)) 
  { 
    for(i = 0; i < buffer && userInput[i] != '\0'; i++) 

    if(userInput[i] == '\n') 
    break; 

    if(strlen(userInput) > buffer || strlen(userInput) < buffer) 
    { 
      int newBuffer = strlen(userInput); 
      userInput = realloc(userInput, newBuffer); 
    }
    userInput[i] = '\0'; 
    return userInput; 
  } 
  else
  {
    return(NULL); 
  } 
} 
