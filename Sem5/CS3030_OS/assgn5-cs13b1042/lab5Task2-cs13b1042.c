// Task2. Implemented by Akilesh B, CS13B1042.

// Required header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

int main()
{
	int d, count = -1, count1 = 0, count2 = 0;			// used for parsing the file 
	char *test = malloc(sizeof(char) * 128);		// variable to check if the PID is obtained in the File after parsing
	char *c = malloc(sizeof(char) * 5);

	int pid;
	printf("Enter pid:");
	scanf("%d", &pid);

	bool flag;

	char p[100];
	sprintf(p, "/proc/%d/", pid);				//checking if the path of the pid folder exits or not
	DIR* dir = opendir(p);						// opening a directory
	
	if(dir){							// PID exists
		flag=true;											
		closedir(dir);
	}
	else if(ENOENT == errno){
		flag=false;
		printf("Invalid PID\n");	//PID doesn't exist.									
	}
	else{
		flag=false;
		printf("Permission Denied\n");		//PID exists but we cannot access it.
	}
	
	if(pid>0 && flag)
	{	
	char str[15];
	sprintf(str, "%d", pid);

	char command[100];
	//The flag -p shows PIDs, -s shows parent process of specified process, -A uses ASCII characters to draw the tree
	strcpy(command, "pstree -p -s -A ");		//pstree command

	strcat(command, str);						// executing command

	strcat(command , " 1>d.txt 2>&1");
	FILE *fp = fopen("d.txt", "ab+");

	int out = system(command);				// system command executes command

	if(out < 0)
	{
		printf("command not executed properly\n");
	}
	else {

		/* This part traverses the file and prints the ancestors of the process with given PID, traverses till it encounters given PID */
		while((d = fgetc(fp)) != EOF){	
		*c  = (char) d;	
	
		if(isdigit(*c)){			//check if its a digit or not
			printf("%s", c);		
		if(count1 == 1){
			strcat(test,c);
			if(strcmp(test, str) == 0){				//checking if the test matches with str
				printf(")\n");				
				break;			
			} 
		}
		else{
			strcpy(test,c);
			count1 = 1;
			if(strcmp(test , str) == 0){			//check test and str
				printf(")\n");				
				break;			
			} 		
		}	
	}
	else if((strcmp(c,"-") == 0)){
		count2 ++;
	}
	else if((strcmp(c,"(") == 0) || (strcmp(c,")") == 0)){
		count1 = -1;
		printf("%s", c);
		if(count == 1){
			printf("\n");
			count = 0;				//To check presence of ). 
		}
		else{
			count = 1;				
		}
	}
	else{
		if(count2 == 1){			
		if((strcmp(c,"+") == 0)) {
			printf("Not a process. It's a thread.\n");
			return 0;
		}
		else{			
			count2 = 0;
		}
		}
		printf("%s", c);
			}
		
	}
}
}
}