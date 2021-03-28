/*
 * File: driver2.c 
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "myio.h"
#define buffer 100

/*
 * Second one (say driver2.c) gets two command-line arguments: 
 * driver2 input_file.txt output_file.txt 
 * input_file.txt is a text file containig many lines. 
 *
 * Your program reads each line from the input file and removes 
 * the extra space characters between the words in that line. 
 * Then it prints the new line into output_file.txt.  
 * So there will be at most one space character between the words in 
 * each line of output_file.txt. 
 * Remove all the space characters from the begining and end of the string as well. 
 * Leave only one space character between the words. 
 * Use only the space character as a seperator, so any other sequence of charcters 
 * are treated like a word (e.g., "   aa    3c   %$3a    " has three words, 
 * after removing the extra space characters you should have "aa 3c %$3a") 
 */ 

int main(int argc, char *argv[])
{
  // YOU NEED TO IMPLEMENT THIS (USE FUNCTIONS FROM myio.h) 
  // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE 

  if(argc != 3)
   {
       fprintf(stderr, "ERROR: Incorrect input please enter three arguments\n");
       fprintf(stderr, "For Example: <program_name> <input_file> <output_file>\n");
       exit(-1);
   }
  
   FILE *input;
   FILE *output;
   char *lines;
   char end = 0;

   input = fopen(argv[1], "r");
   if(input == NULL)
   {
       fprintf(stderr,"Could Not Find or Open File %s\n", argv[1]);
       exit(-1);
   }

   output = fopen(argv[2], "w");
    if(output == NULL)
   { 
     fclose(input); 
     fprintf(stderr, "Could Not Open and/or Create File %s\n", argv[2]);
     exit(-1);
   } 
   
   while((lines = ReadLineFile(input)) != NULL)
   { 
     char *inputRead = lines; 
     
     while(*lines != '\0') 
     { 
       if(end == ' ' && *lines == ' ') 
       { 
         end = *lines; 
         lines++; 
         continue; 
       } 
       fputc(*lines, output); 
       end = *lines; 
       lines++; 
     } 
     free(inputRead); 
     fputc('\n', output);
   } 
   fclose(input);
   fclose(output); 
   return 0; 
} 

   