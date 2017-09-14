// Multi process sorting. Implemented by Akilesh B, cs13b1042
// Required header files

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>

//struct element containing indexes of the array. It contains the starting and ending elements
struct element
{
	int beg;
	int end;
};

//declaration of input, output arrays and number of elements n
static int *input;
int *output, n;

//merge_sort function
void* merge_sort(void *in)
{
	struct element *var = (struct element*) in;

	int beg = var->beg, end = var->end;

	if(beg == end)		//if beg and end are the same. It means that sorting is completed
		_exit(0);

	struct element var1, var2;

	int mid = (beg+end)/2;
	
	var1.beg = beg; //var1 takes care of sorting the first half of the input array
	var1.end = mid;

	var2.beg = mid+1; //var2 takes care of sorting the second half of the input array
	var2.end = end;

	pid_t child_a, child_b;		//create two children

    child_a = fork();   // child 1 created by parent

    if(child_a == 0)
    {
    	merge_sort((void *) &var1);		//child 1 performs sorting of first half of input array
    	_exit(0);
    } 
    else 
    {
    	child_b = fork();      //child 2 created by parent.

    	if(child_b == 0) 
    	{
			merge_sort((void *) &var2);		//child 2 performs sorting of second half of child array
			_exit(0);
		}
    	else 
    	{
    		wait(NULL);       // Parent waiting for both the children to execute first.
    		wait(NULL);
    	
    	//parent waits for both its children to finish their tasks and performs merge functionality
    	int i = beg, j = mid+1, k = beg;

    	//merging two arrays
		while(i <= mid && j <= end)		//as long as i is less than mid and j is less than end
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
	printf("enter the number of elements to be sorted\n");
	scanf("%d", &n);

	//allocate memory for input array based on n inputted
	input = (int*) malloc(sizeof(int) * n);

	//creating a shared memory so that input array is accessbile to all the children
	input = mmap(NULL, sizeof *input, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	 int random_num;
	 time_t t;
	 	
	 srand( (unsigned)time(&t)+10);     
	 int i;

	 //generating random numbers for input array
	 for(i=0; i< n; i++)
	 {
		 random_num = rand()%10000;
		 input[i] = random_num;			//generating random numbers
	 }

	 printf("Generated input array is \n");
	 for(i=0; i<n; i++)
	 {
	 	printf("%d\n", input[i]);
	 }

	 //allocating memory for output array
	output = (int*) malloc(sizeof(int) * n);

	struct element init;

	init.beg = 0;
	init.end = n-1;

	struct timeval timevar;

	gettimeofday(&timevar, NULL);
	long long t1 = timevar.tv_usec;		//getting time before sorting

	merge_sort((void *) &init);		//calling the merge_sort function with range of array indexes

	gettimeofday(&timevar, NULL);
	long long t2 = timevar.tv_usec;		//getting time after sorting

	printf("time taken for multi process sorting = %lld usec\n", t2-t1);
	
	for(i=0; i<n; i++)
	{
		printf("%d ", input[i]);	//printing the output array
	}
	printf("\n");

	return 0;   
}