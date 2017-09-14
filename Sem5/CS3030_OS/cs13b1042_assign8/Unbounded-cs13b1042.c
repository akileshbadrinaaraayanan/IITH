#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#define max 100

//to find exponential distribution around mean
int ex(int a)
{
	int var = (-1)*((rand() % 70) + 1)/a;
	int x = 1000* (exp(var));
	x = x/a;
	return x;
}

struct arguments
{
	int cur;
	int n, k, dur1, dur2;
};

long long answer = 0;
int count = 0;

void *testCS(void *curvar)
{
	struct arguments *thisvar = (struct arguments *) curvar;

	struct tm *reqEnterTime;
	struct tm *actEnterTime, *exitTime;

	pthread_t id = pthread_self();
	int l;
	struct timeval timevar;

	time_t current;
	time(&current);

	for(l=0; l < thisvar->k; l++)
	{
		count++;

		gettimeofday(&timevar, NULL);
		long long t1 = timevar.tv_usec;

		reqEnterTime = localtime(&current);

		printf("%d th CS Request at %s by the thread %lu , ID = %d\n", l, asctime(reqEnterTime), id, thisvar->cur);

		int lock = 0;

		//Entry section
		while(__sync_val_compare_and_swap(&lock, 0, 1) != 0)
			;

		//Critical Section
		gettimeofday(&timevar, NULL);
		long long t2 = timevar.tv_usec;

		actEnterTime = localtime(&current);

		long long t3 = t2-t1;
		answer = answer + t3;

		printf("%d th CS Entry at %s by the thread %lu , ID = %d\n", l, asctime(actEnterTime), id, thisvar->cur);
		sleep(thisvar->dur1);

		lock = 0;

		// Remainder section
		exitTime = localtime(&current);

		printf("%d th CS Exit at %s by the thread %lu , ID = %d\n", l, asctime(exitTime), id, thisvar->cur);
		sleep(thisvar->dur2);
	}
	return NULL;
}

struct arguments *data[max];

int main()
{
	int n;
	int k, avg1, avg2;

	FILE *fp;
	fp = fopen("input.txt", "r"); 		//read input from file

	if (fp == NULL)
	{   
		printf("error in opening the file\n"); 
        exit(-1);
    }
	
	fscanf(fp, "%d %d %d %d", &n, &k, &avg1, &avg2);		//get input
	
	pthread_t nthread[n];	//n threads

	int i;
	for (i = 0; i < n; i++)
	{
		data[i] =(struct arguments *) malloc (sizeof(struct arguments));
		data[i]->n = n;
		data[i]->k = k;
		data[i]->cur = i;
		data[i]->dur1 = ex(avg1);
		data[i]->dur2 = ex(avg2);

		pthread_create(&nthread[i], NULL, testCS, data[i]);		//create thread
	}

	for (i = 0; i < n; i++)
	{
		pthread_join(nthread[i], NULL);			//join
	}

	printf("average time to enter CS: %lld\n", answer/count);

	return 0;
}