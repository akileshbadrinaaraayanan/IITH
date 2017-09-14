/* C program to delete all subdirectories and files using system calls  */
/* Written by Akilesh B, CS13B1042  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Below is the function which appends a single character to already existing string */
char *appendChar(const char *start, char c) 
{
    size_t size = strlen(start);
    char *modstr = malloc(size + 2);    //allocate space
    strcpy(modstr, start);
    modstr[size] = c;
    modstr[size + 1] = '\0';
    return modstr;          //appended string
}

/*To reverse the contents of input file and output file line wise */
void reverseOrder(int inFile, int outFile) {
  char dt;
  char* line="";  			// keeps track of every line 
  while (read(inFile, &dt, 1) == 1) {
    if(dt == '\n')  	 	// till as long as new line is encountered.
    {   
    line = appendChar(line,dt);  			// keep appending
    reverseOrder(inFile, outFile);			// recursive call
    write(outFile, line, strlen(line));			//write the line to output file in reverse order
    return;
    }
    else {								// if it is not the end of line
      line = appendChar(line,dt);			//just append
    }
  }
return;
}

/* main function which takes in input file and output file as arguments */
/* output file is argv[1], input file is argv[2]  */
/* Run it as ./a.out t1-out.txt t1-inp.txt  */

int main(int argc, char **argv) {
	if(argc != 3)
		return -1;

  int outFile = open(argv[1], O_WRONLY | O_APPEND | O_CREAT);     //output file
  int inFile = open(argv[2], O_RDONLY);                           //input file
  if(inFile < 0)
    {
    printf("error opening input file\n");
  	return -1;
  }
  reverseOrder(inFile, outFile);                  // call function
  close(inFile);
  close(outFile);
  return 0;
}

