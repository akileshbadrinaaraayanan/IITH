#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>

struct element
{
	int beg;
	int end;
};

static int *input;
int *output, n;

void* merge_sort(void *in)
{
	struct element *var = (struct element*) in;

	int beg = var->beg, end = var->end;

	if(beg == end)
		_exit(0);

	struct element var1, var2;

	int mid = (beg+end)/2;
	
	var1.beg = beg; 
	var1.end = mid;

	var2.beg = mid+1;
	var2.end = end;

	pid_t child_a, child_b;

    child_a = fork();   // child 1 created by parent

    if(child_a == 0)
    {
    	merge_sort((void *) &var1);
    	_exit(0);
    } 
    else 
    {
    	child_b = fork();      //child 2 created by parent.

    	if(child_b == 0) 
    	{
			merge_sort((void *) &var2);
			_exit(0);
		}
    	else 
    	{
    		wait(NULL);       // Parent waiting for both the children to execute first.
    		wait(NULL);
    	
    	int i = beg, j = mid+1, k = beg;

		while(i <= mid && j <= end)
		{
			if(input[i] < input[j])
				output[k++] = input[i++];
		
			else
				output[k++] = input[j++];
		}

		for(; i <= mid; i++)
			output[k++] = input[i];

		for(; j <= end; j++)
			output[k++] = input[j];

		for(i = beg; i <= end; i++)
			input[i] = output[i];
    	}
    }

    return NULL;
  }

	

int main () 
{
	long long time_array[19]={0}, t3;
	int counter = 0, i;
	
	for (n = 100; n <= 1000; n = n+50)
	{	
	printf("n is %d\n", n);
	input = (int*) malloc(sizeof(int) * n);

	input = mmap(NULL, sizeof *input, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	 int random_num;
	 time_t t;
	 	
	 srand( (unsigned)time(&t)+10);     
	

	 for(i=0; i< n; i++)
	 {
		 random_num = rand()%10000;
		 input[i] = random_num;	
	 }

	output = (int*) malloc(sizeof(int) * n);

	struct element init;

	init.beg = 0;
	init.end = n-1;

	struct timeval timevar;

	gettimeofday(&timevar, NULL);
	long long t1 = timevar.tv_usec;

	merge_sort((void *) &init);

	gettimeofday(&timevar, NULL);
	long long t2 = timevar.tv_usec;

	t3 = t2-t1;

	time_array[counter] = t3;
	counter++;

	/*for(i=0; i<n; i++)
	{
		printf("%d ", input[i]);
	}
	printf("\n");
	*/
	
	}
	

	for(i=0; i<counter; i++)
	{
		printf("time array element %d is %llu\n", i, time_array[i]);
	}

	return 0;   
}