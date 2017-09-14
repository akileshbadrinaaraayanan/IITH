#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct element
{
	int beg;
	int end;
};

int *input, *output, n;

void* merge_sort(void *in)
{
	struct element *var = (struct element*) in;

	int beg = var->beg, end = var->end;

	if(beg == end)
		pthread_exit(0);

	struct element var1, var2;
	pthread_t thread1, thread2;

	int mid = (beg+end)/2;
	
	var1.beg = beg; 
	var1.end = mid;

	var2.beg = mid+1;
	var2.end = end;

	int ret_val1, ret_val2;

	ret_val1 = pthread_create(&thread1, NULL, merge_sort, (void *)&var1);
	
	if(ret_val1 > 0)
		printf("Error in creating thread 1");

	ret_val2 = pthread_create(&thread2, NULL, merge_sort, (void *)&var2);

	if(ret_val2 > 0)
		printf("Error in creating thread 2");

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

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
	
	pthread_exit(0);
	
	return NULL;
}

int main()
{
	long long time_array[19]={0}, t3;
	int counter = 0, i;

	for(n=100; n<=1000; n=n+50)
	{
	printf("n is %d\n", n);

	input = (int*) malloc(sizeof(int) * n);
	
	int random_num;
	time_t t;
	 	
	srand( (unsigned)time(&t)+10);     //seeding

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
	pthread_t init_thread;

	gettimeofday(&timevar, NULL);
	long long t1 = timevar.tv_usec;

	pthread_create(&init_thread, NULL, merge_sort, &init);
	pthread_join(init_thread, NULL);

	gettimeofday(&timevar, NULL);
	long long t2 = timevar.tv_usec;

	t3 = t2-t1;

	time_array[counter] = t3;
	counter++;

	/*for(i=0; i<n; i++)
	{
		printf("%d ",  input[i]);
	}
	printf("\n");
	printf("\n");
	*/
}	

	for(i=0; i<counter; i++)
	{
		printf("time array element %d is %llu\n", i, time_array[i]);
	}
	return 0;
}
