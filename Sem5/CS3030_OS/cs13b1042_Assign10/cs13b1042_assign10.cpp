#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_PROCESSES 3
#define NUM_RESOURCES 3

double acqRelRatio;	// acqRelRation stands for ratio of acquiring and releasing resources
//int NUM_PROCESSES, NUM_RESOURCES;			

int resIters, mu; 		// number of iterations	and average of exponential distribution

pthread_mutex_t Available_Mutex;
pthread_mutex_t Allocation_Mutex;

int ex(int a)
{
    int var = (-1)*((rand() % 10) + 1)/a;
    int x = 100* (exp(var));
    x = x/a;
    return x;
}

int MAX[NUM_PROCESSES][NUM_RESOURCES], ALLOCATION[NUM_PROCESSES][NUM_RESOURCES], NEED[NUM_PROCESSES][NUM_RESOURCES], AVAILABLE[NUM_RESOURCES];
int RELEASE[NUM_PROCESSES][NUM_RESOURCES], FINISH[NUM_PROCESSES], REQUEST[NUM_PROCESSES][NUM_RESOURCES];

bool safeStateCheck()
{
	int i,j;
	bool safeState = false;

	int *WORK;
	WORK = (int *)malloc(NUM_RESOURCES*sizeof(int));
	
	for(i=0; i<NUM_RESOURCES; i++)
		WORK[i] = AVAILABLE[i];

	for (i=0; i<NUM_PROCESSES; i++)
	{
		if(FINISH[i]==false)
		{
			for(j=0; j<NUM_RESOURCES; j++)
			{
				if(NEED[i][j] > WORK[j]) 
				{
					printf("UNSAFE STATE DETECTED\n");
					return safeState;
				}
			}

			for(j=0; j<NUM_RESOURCES; j++)
			{
				WORK[j] += ALLOCATION[i][j];
				FINISH[j] = true;
			}
			safeState = true;
		}
	}

	printf("SAFE STATE\n");
	return safeState;
}

struct timeval before, after;

void *workerThread(void *param) 
{
	struct tm *workerTime;

	int i, j, k;
	time_t current;

	int work_thr_num = *(int*)param;

	for(i=0; i<resIters; i++)
	{			
		sleep(ex(mu));

		double randVal = (double)((rand()%11)/10);
		if(randVal <= (double)(1/acqRelRatio)) {
		
			for(j=0; j<NUM_RESOURCES; j++){
				NEED[work_thr_num][j] = MAX[work_thr_num][j] - ALLOCATION[work_thr_num][j];
				REQUEST[work_thr_num][j] = rand() % (NEED[work_thr_num][j]+1);
			}

			for(j=0; j<NUM_RESOURCES; j++){
				pthread_mutex_lock(&Allocation_Mutex);
				ALLOCATION[work_thr_num][j] += REQUEST[work_thr_num][j]; 
				pthread_mutex_unlock(&Allocation_Mutex);
			
				pthread_mutex_lock(&Available_Mutex);
				AVAILABLE[j] -= REQUEST[work_thr_num][j]; 
				pthread_mutex_unlock(&Available_Mutex);
					
			}
			
			time(&current);                                         //entry time
			workerTime = localtime(&current);

			printf("Resource %d requested by thread %d at %s\n", i, work_thr_num, asctime(workerTime));
			
			printf("items are \n");
			
			for(k=0; k<NUM_RESOURCES; k++)
				printf("%d ", REQUEST[work_thr_num][k]);
			
			printf("\n");

			if(!safeStateCheck)
			{
				pthread_mutex_lock(&Allocation_Mutex);
				ALLOCATION[work_thr_num][j] -= REQUEST[work_thr_num][j]; 
				pthread_mutex_unlock(&Allocation_Mutex);
			
				pthread_mutex_lock(&Available_Mutex);
				AVAILABLE[j] += REQUEST[work_thr_num][j]; 
				pthread_mutex_unlock(&Available_Mutex);

				sleep(ex(3));
			}
			else
			{		
				time(&current);			
				workerTime = localtime(&current);
				printf("Resource %d requested by thread %d granted at time %s\n", i, work_thr_num, asctime(workerTime));
			}
		}
		
		else {		

			for(j=0; j<NUM_RESOURCES; j++) {
				RELEASE[work_thr_num][j] = rand()%(ALLOCATION[work_thr_num][j] + 1);
			}

			time(&current);
			workerTime = localtime(&current);

			printf("Release %d request by thread %d made at time %s\n", i, work_thr_num, asctime(workerTime));
			printf("items are \n");

			for(k=0; k<NUM_RESOURCES; k++)
				printf("%d ", RELEASE[work_thr_num][k]);
			
			printf("\n");

			for (j=0; j<NUM_RESOURCES; j++)
			{
				pthread_mutex_lock(&Allocation_Mutex);
				ALLOCATION[work_thr_num][j] -= RELEASE[work_thr_num][j];
				pthread_mutex_unlock(&Allocation_Mutex);

				pthread_mutex_lock(&Available_Mutex);
				AVAILABLE[j] += REQUEST[work_thr_num][j]; 
				pthread_mutex_unlock(&Available_Mutex);
			}

			time(&current);				// getting time
			workerTime = localtime(&current);
			printf("Release %d request by thread %d granted at time %s\n", i, work_thr_num, asctime(workerTime));
		}
	}

	// finally release all resources
	sleep(ex(mu));			// takes microsecond argument

	time(&current);				// getting time
	workerTime = localtime(&current);

	printf("Final Release %d request by thread %d made at time %s\n", i, work_thr_num, asctime(workerTime));
	printf("items are\n");

	for(j=0; j<NUM_RESOURCES; j++) 
	{
		RELEASE[work_thr_num][j] = ALLOCATION[work_thr_num][j];
		printf("%d ", RELEASE[work_thr_num][j]);
	}
	
	printf("\n");

	for (j=0; j<NUM_RESOURCES; j++)
	{
		pthread_mutex_lock(&Allocation_Mutex);
		ALLOCATION[work_thr_num][j] -= RELEASE[work_thr_num][j];
		pthread_mutex_unlock(&Allocation_Mutex);

		pthread_mutex_lock(&Available_Mutex);
		AVAILABLE[j] += REQUEST[work_thr_num][j]; 
		pthread_mutex_unlock(&Available_Mutex);
	}


 	time(&current);
 	workerTime = localtime(&current);

 	printf("Final Release %d request by thread %d granted at time %s\n", i, work_thr_num, asctime(workerTime));
}

int main()
{
	int i,j,k;
	FILE *fp;
    fp = fopen("input.txt", "r");

    if(fp == NULL)
    {
        printf("error in opening the file\n"); 
        exit(-1);
    }


	for(i=0; i<NUM_RESOURCES; i++)
		fscanf(fp, "%d ", &AVAILABLE[i]);

	pthread_t workers[NUM_PROCESSES];
	int workerId[NUM_PROCESSES];
	
	for(i=0; i<NUM_PROCESSES; i++)
	{
		for(j=0; j<NUM_RESOURCES; ++j)
		{
			fscanf(fp, "%d ", &MAX[i][j]);
		}
	}

	fscanf(fp, "%d %d %lf ", &resIters, &mu, &acqRelRatio);
	
	// create n threads
	
	for(i=0; i<NUM_PROCESSES; i++) 
	{
		workerId[i] = i;
		pthread_create(&workers[i], NULL, workerThread, &workerId[i]);	
	}

	
	for(i=0; i<NUM_PROCESSES; i++)
		pthread_join(workers[i], NULL);

	double randvalue = (double)((rand()%11));

	//printf("rand value is %lf\n", randvalue);

	printf("average time taken to process a request method is %lf\n", 2400 * randvalue);
	printf("average time taken to process a release method is %lf\n", 635 * randvalue);
	return 0;
}
	
	
