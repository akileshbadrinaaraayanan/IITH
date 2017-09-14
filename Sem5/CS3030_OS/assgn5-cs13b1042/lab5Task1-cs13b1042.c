/* Lab5-Task1, Implemented by Akilesh B, CS13B1042 */

// Required header files
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>

// To check if the element already exists in the array. This is to make sure that elements of randomly generated array is unique.
int already_exists(int arr[], int loc, int random_num)
{
    int j;
    for(j = 0; j < loc; j++)
    {
        if(arr[j] == random_num)    //check if already present
        {
            return 0;
        }
    }
    return 1;
}

// Function to generate a random number by taking num and max as arguments. 
int * generate_random(int m, int n)
{
	 static int a[10];         //array containing random numbers
	 int i, random_num;

	 time_t t;
	 	
	 srand( (unsigned)time(&t)+10);     //seeding

	 for(i=0; i< m;)
	 {
		 random_num = (rand()%n) + 1;
		 
		 if(already_exists(a, i+1, random_num))   //check if already exists
		 {
		 	a[i] = random_num;
		 	i++;
		 }	
	 }

	 return a;
}

 static unsigned long long *glob_var1;  // creating shared memory which can be accessed by all the processes
 static unsigned long *glob_var2;

int main () {
	
     int num, max;
	
	printf("num:");    
	scanf("%d", &num);     // getting num from the user
	printf("max:");
	scanf("%d", &max);     // getting max from the user
	
	 glob_var1 = mmap(NULL, sizeof *glob_var1, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	 glob_var2 = mmap(NULL, sizeof *glob_var2, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    unsigned long long mul = 1;
    unsigned long sum = 0;

    pid_t child_a, child_b;

    child_a = fork();   // child 1 created by parent

    if(child_a == 0)
    {
    	int *p2;
    	
    	p2 = generate_random(num, max);        //generating random numbers

    	int j;
    	
    	printf("Output of c1: ");

    	for(j = 0; j < num; j++)
    	{
    		printf("%d ", *(p2+j));
        	mul = (mul * (*(p2+j)));       //multiply
    	}

    	
    	*glob_var1 = mul;          //storing it in shared memory
    	printf("prod: %llu\n", *glob_var1);

    } else {
    	child_b = fork();      //child 2 created by parent.

    	if(child_b == 0) {
	
    		int *p1;
    		p1 = generate_random(num, max);       //generating random numbers

    		int j;

    		printf("Output of c2: ");

    		for(j = 0; j < num; j++)
    		{
    			printf("%d ", *(p1+j));
        		sum = sum + *(p1 + j);            //sum
    		}
    		*glob_var2 = sum;                     // storing it in shared memory
    		printf("sum: %lu\n", *glob_var2);
    	}
    	else {
    		wait(NULL);       // Parent waiting for both the children to execute first.
    		wait(NULL);
    		printf("Output of p: %llu\n", ((*glob_var1) - (*glob_var2))); // parent printing difference of product and sum
    		munmap(glob_var1, sizeof *glob_var1);         // deallocating memory allocated to glob var1
    		munmap(glob_var2, sizeof *glob_var2);         // deallocating memory allocated to glob var2
    	}
    }

    return 0;
}