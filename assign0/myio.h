/*
 * File: myio.h
 * Version: 1.0
 * Last modified on Sat Jan 26 18:59:26 2019 by korkmaz
 * This is mostly from a similar file provided by eroberts
 * -----------------------------------------------------
 * This interface provides access to a basic library of
 * functions that simplify the reading of input data.
 */

#ifndef _myio_h
#define _myio_h

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
int ReadInteger(int numInts);

/*
 * Function: findMaxInt
 * Usage: maxInt = findmaxInt(intArray, &userInts);
 * ---------------------
 * findMaxInt takes in an array of user inputted integers and
 * iterates through and keeps track of the largest integer then
 returns it.
 */
int findMaxInt(int userIntArray[], int *userNums); 



/* 
 * Function: findMaxDouble 
 * Usage: maxDouble = findMaxDouble(doubleArr, &userDoubles); 
 * findMaxDouble takes in an array of user inputted doubles and 
 * iterates through and keeps track of the largest doubles then
 * returns it. 
 */

double findMaxDouble(double userDoubleArray[], int *userDoubles);



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
double ReadDouble(int numDoubles); 


/*
* Function: LongestLine 
* Usage: s = LongestLine();
* -------------------------
* LongestLine takes in the number of lines from stdin
and reads in the given input strings/lines. The functions iterates
through the lines  
*/
char *LongestLine(int size);



/* 
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline character that terminates
 * the input is not stored as part of the string.
 */
char *ReadLine(void);



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
 * Note: the above ReadLine(); can simply be implemented as 
 *  char *ReadLine() { return(ReadLineFile(stdin)); } 
 */
char *ReadLineFile(FILE *infile);

#endif
