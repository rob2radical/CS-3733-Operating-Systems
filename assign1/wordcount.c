/*
 * File: wordcount.c
 * Robert Quintanilla qsy775
 *
 * ....
 */

#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// include other standard header files needed 

// if you like copy your myio.h and myio.c here and use it when reading lines from a file!
// #include "myio.h"  

/*
 * YOUR COMMENTS
 */

int countWords(char *input_file);

int main(int argc, char *argv[])
{
  // YOU NEED TO IMPLEMENT THIS + some other functions! 

  int i;
  pid_t pid;
  int words;
  int countedFiles = 0;
  int failedFiles = 0;
  int fd[2];
  int status;

  if(argc < 2) // if user does not specify the right program file or does not enter at least one text file
  { 
    fprintf(stderr, "ERROR: Invalid Arguments, Usage: [./exename] [file_1] [file_2] ... [file_n]\n");
    exit(-1);
  } 
  for(i = 1; i < argc; i++) // create child processes for each of the text files given
  {  
    pipe(fd); // to pass the value of failedFiles from child process to parent process
    pid = fork(); 
    
    if(pid == 0) 
    { 
      close(fd[0]);
      int temp; // used to read from pipe 
      words = countWords(argv[i]); 
      if(words >= 0) 
      { 
        temp = 0;
        printf("Child process %ld for %s: number of words is %d\n", (long)getpid(), argv[i], words); 
        if (write(fd[1], &temp, sizeof(int)) == -1) 
        {
          fprintf(stderr, "ERROR: Could not write temp integer\n");
          return -1;
        }
        close(fd[1]);
        return EXIT_SUCCESS; 
      } 
      else if(words == -1)  // if file could not be found
      { 
        temp = 1; 
        printf("Child process %ld for %s: does not exist\n", (long)getpid(), argv[i]);
        if (write(fd[1], &temp, sizeof(int)) == -1){
          fprintf(stderr, "ERROR: Could not write temp integer\n");
          return -1;
        }
        close(fd[1]);
        return -1;
      } 
    }
    else
    {
      close(fd[1]);
      int temp; // used to write # of failed files from pipe to parent process
      if (read(fd[0], &temp, sizeof(int)) == -1)
      {
        fprintf(stderr, "ERROR: Could not read temp integer\n");
        return -1;
      }
      failedFiles += temp;
      close(fd[0]);
    }
  } 
  for(i = 0; i < argc - 1; i++) // to have the parent process wait for each of its children to terminate
  { 
    pid = wait(&status); 
    
    if(WEXITSTATUS(status) == 0 && pid > 0) 
    { 
      countedFiles++; 
    } 
  } 
  printf("Main process created %d child processes to count words in %d files\n", argc - 1, argc - 1); 
  printf("%d have been counted successfully\n", countedFiles); 
  printf("%d files did not exist\n", failedFiles); 
  return 0;
}

int countWords(char *input_file) // function to count the number of words from a given text file
{ 
  FILE *input; 
  int wordCount = 0; 
  //char line[20];
  char ch;

  input = fopen(input_file, "r"); 
  
  if(input == NULL) 
  { 
    return -1;
  } 
  /*while(fscanf(input, "%s", line) != EOF) 
  { 
    wordCount++; 
  } 
  fclose(input); 
  return wordCount;*/ 


  bool previsspace = 1;
  while((ch = getc(input)) != EOF) 
  { 
    if(isspace(ch) == 0 && (previsspace == 1)) 
      wordCount++;
      previsspace = isspace(ch);
  } 
  fclose(input);
  return wordCount;
}


   