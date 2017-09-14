/* Multi threaded sorting. Implemented by Akilesh B, cs13b1042 */
/* Required header files */
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct element 			//struct containing the starting and ending indexes
{
	int beg;
	int end;
};


int *input, *output, n;		//input array, output array and n


void* merge_sort(void *in)		//merge_sort function
{
	struct element *var = (struct element*) in;

	int beg = var->beg, end = var->end;

	if(beg == end)			//if beg and end are the same, then sorting is done
		pthread_exit(0);

	struct element var1, var2;	
	pthread_t thread1, thread2;

	int mid = (beg+end)/2;		//mid element
	
	var1.beg = beg; 		//var1 takes care of first half of the array
	var1.end = mid;

	var2.beg = mid+1;		//var2 takes care of second half of the array
	var2.end = end;

	int ret_val1, ret_val2;

	ret_val1 = pthread_create(&thread1, NULL, merge_sort, (void *)&var1);	//create a thread for first half of the array
	
	if(ret_val1 > 0)	//pthread_create returns 0 on success
		printf("Error in creating thread 1");

	ret_val2 = pthread_create(&thread2, NULL, merge_sort, (void *)&var2);	//create another thread for second half of the array

	if(ret_val2 > 0)	//pthread_create returns 0 on success
		printf("Error in creating thread 2");

	//pthread_join waits for both the threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	
	//merge functionality
	int i = beg, j = mid+1, k = beg;

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
	
	pthread_exit(0);		//finally exit after merging is succesful
	
	return NULL;
}

int main()
{
	printf("enter the number of elements to be sorted\n");
	scanf("%d", &n);
	//allocate memory for input array based on element n inputted
	input = (int*) malloc(sizeof(int) * n);
	
	int random_num;
	time_t t;
	 	
	srand( (unsigned)time(&t)+10);     //seeding
	int i;

	for(i=0; i< n; i++)
	 {
		 random_num = rand()%10000;
		 input[i] = random_num;		//randomly generate the elements of the input array
	 }

	 printf("Generated input array is \n");
	 for(i=0; i<n; i++)
	 {
	 	printf("%d\n", input[i]);		//print the input array
	 }

	output = (int*) malloc(sizeof(int) * n);		//allocate space for the output array

	struct element init;

	init.beg = 0;
	init.end = n-1;

	struct timeval timevar;		//this structure contains time var
	pthread_t init_thread;		//main thread which calls for the entire range of values

	gettimeofday(&timevar, NULL);
	long long t1 = timevar.tv_usec;		//time before sorting

	pthread_create(&init_thread, NULL, merge_sort, &init);
	pthread_join(init_thread, NULL);

	gettimeofday(&timevar, NULL);
	long long t2 = timevar.tv_usec;		//time after sorting

	printf("time taken for multi threaded sorting = %lld usec\n", t2-t1);
	
	for(i=0; i<n; i++)
	{
		printf("%d ", input[i]);		//sorted array is printed
	}
	printf("\n");

	return 0;
}
