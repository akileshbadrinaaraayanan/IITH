/* Required header files */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>

#define max 100

//function to calculate exponential distribution around mean
int ex(int a)
{
	int var = (-1)*((rand() % 70) + 1)/a;
	int x = 1000* (exp(var));
	x = x/a;
	return x;
}

//Struct containing required arguments: id, n, k, wait times
struct arguments
{
	int cur;
	int n, k, dur1, dur2;
};

int waiting[max];
long long answer = 0;
int count = 0;

//function to testCS
void *testCS(void *curvar)
{
	struct arguments *thisvar = (struct arguments *) curvar;	

// Reqtime, actTime and exitTime structures
	struct tm *reqEnterTime;
	struct tm *actEnterTime, *exitTime;

	pthread_t id = pthread_self();		//id of thread
	int l, m;
	struct timeval timevar;

	time_t current;
	time(&current);			//current time

	for(l=0; l < thisvar->k; l++)
	{
		count++;

		waiting[thisvar->cur] = 1;
		int lock = 0;
		int key = 1;

		gettimeofday(&timevar, NULL);
		long long t1 = timevar.tv_usec;		//measure time t1 when request is placed

		reqEnterTime = localtime(&current);

		printf("%d th CS Request at %s by the thread %lu , ID = %d\n", l, asctime(reqEnterTime), id, thisvar->cur);

		//Entry section
		while(key && waiting[thisvar->cur])
			key = __sync_val_compare_and_swap(&lock, 0, 1);

		waiting[thisvar->cur] = 0;

		//Critical Section
		gettimeofday(&timevar, NULL);
		long long t2 = timevar.tv_usec;		//measure time t2 when it enters its CS

		actEnterTime = localtime(&current);

		long long t3 = t2 - t1;		//Time taken to enter CS 
		answer = answer + t3;		//For measuring average time taken to measure CS

		printf("%d th CS Entry at %s by the thread %lu , ID = %d\n", l, asctime(actEnterTime), id, thisvar->cur);
		sleep(thisvar->dur1);		//sleep for dur1

		m = ((thisvar->cur) + 1) % (thisvar->n);

		while(!waiting[m] && (m != thisvar->cur))
			m = (m+1) % (thisvar->n);
		
		if(thisvar->cur == m)
			lock = 0;
		else
			waiting[m] = 0;

		// Remainder section
		exitTime = localtime(&current);

		printf("%d th CS Exit at %s by the thread %lu , ID = %d\n", l, asctime(exitTime), id, thisvar->cur);
		sleep(thisvar->dur2);		//sleep for dur2
	}
	return NULL;
}

struct arguments *data[max];		//argument structure array 

int main()
{
	int n;
	int k, avg1, avg2;

	FILE *fp;
	fp = fopen("input.txt", "r"); 	//open input from txt file

	if (fp == NULL)
	{   
		printf("error in opening the file\n"); 
        exit(-1);
    }
	
	fscanf(fp, "%d %d %d %d", &n, &k, &avg1, &avg2);
	
	pthread_t nthread[n];

	int i;
	for (i = 0; i < n; i++)		//getting n, k, lambda1, lambda2 for each data
	{
		data[i] =(struct arguments *) malloc (sizeof(struct arguments));
		data[i]->n = n;
		data[i]->k = k;
		data[i]->dur1 = ex(avg1);
		data[i]->dur2 = ex(avg2);
		data[i]->cur = i;

		pthread_create(&nthread[i], NULL, testCS, data[i]);		//create thread which executes on testCS
	}

	for (i = 0; i < n; i++)
	{
		pthread_join(nthread[i], NULL);		//all threads join
	}

	printf("average time to enter CS: %lld\n", answer/count);

	return 0;
}