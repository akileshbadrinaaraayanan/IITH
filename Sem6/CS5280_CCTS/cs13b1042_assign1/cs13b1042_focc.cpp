/* Required header files */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <atomic>
#define max 1000000

using namespace std;

//function to calculate exponential distribution around mean
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
	int n, m, constVal, dur1;
};

int *sharedMem, m, n;
pthread_mutex_t lock;
pthread_mutex_t pLock;
struct timeval timevar;

//Buffer, readset and writeset for every transaction

//int *abortCount = new int[n];
int abort_answer = 0;

//long long int *commitDelay = new long long int[n]; 
long long int commit_answer = 0;

int** buffer;
int** readSet;
int** writeSet;
int *txStatus;
long long int *txStartTime;
long long int *txEndTime;


class FOCC
{
	volatile atomic<int> id;

public:
	FOCC()
	{
		id = 0;
	}

	int begin_trans();	

	//i is the id in all these cases
	void read(int i, int x, int &l);
	void write(int i, int x, int l);
	int tryC(int i);
};


int FOCC::begin_trans()
	{
		id++;
		return id;
	}

void FOCC::read(int id, int x, int &l)
	{
		pthread_mutex_lock(&lock);
			l = sharedMem[x];
		pthread_mutex_unlock(&lock);
		readSet[id][x] = 1;	
	}

void FOCC::write(int id, int x, int l)
	{
		//pthread_mutex_unlock(&lock);
		buffer[id][x] = l;
		//pthread_mutex_unlock(&lock);

		writeSet[id][x] = 1;
	}

// Everything is same except this function
// Here we compare against all the concurrent live transactions
// Check if writeset of id intersection read set of all the concurrent transactions is null
int FOCC::tryC(int id)
{
	int toAbort = 0;
	pthread_mutex_lock(&lock);
	for(int i=0; i<n; i++)
	{
		//ti has ended before tj has started
		//Compare to all concurrently active transactions
		if(txStatus[i] == 2)	// = 2 => its live
		{
			for(int j=0; j<m; j++)
				{
					if(writeSet[id][j] == readSet[i][j] && readSet[i][j] == 1)
						toAbort = 1;
				}
		}
	}

	if(toAbort == 0)
	{
		txStatus[id] = 1;		//Commit

		for(int j=0; j<m; j++)
		{
			if(writeSet[id][j] == 1)
			{
				sharedMem[j] = buffer[id][j];
			}
		}
	}

	else if(toAbort == 1)
	{
		cout << "transaction aborted " << endl;
		txStatus[id] = -1;		//Abort
		//return -1;
	}
	
	pthread_mutex_unlock(&lock);
	return txStatus[id];
}

FOCC fo;

void *updtMem(void *curvar)
{
	struct arguments *thisvar = (struct arguments *) curvar;	

// Reqtime, actTime and exitTime structures
	struct tm *readTime, *writeTime, *commitTime;

	int status = -1;       //  -1 => ABORT
	int abortCnt = -1;		//keeps track of abort count
	int m = thisvar->m;
	int thid = thisvar->cur;

	time_t current;
	time(&current);

	gettimeofday(&timevar, NULL);
	long long critStartTime = timevar.tv_usec;
	
	//FOCC fo;
	pthread_mutex_lock(&pLock);
	cout << "thread id is " << thid << endl;
	pthread_mutex_unlock(&pLock);

	buffer[thid] = new int[m];
	readSet[thid] = new int[m];
	writeSet[thid] = new int[m];
	
	do
	{
		txStartTime[thid] = 0;
		//txEndTime[thid] = 0;

		for(int j=0; j<m; j++)
		{
			readSet[thid][j] = 0;
			writeSet[thid][j] = 0;
			buffer[thid][j] = 0;
		}
		
		int tid = fo.begin_trans();
		txStatus[thid] = 2;			//Tx is live

		gettimeofday(&timevar, NULL);
		txStartTime[thid] = timevar.tv_usec;
	
		int locVal;
			
		srand(time(NULL));
		//time_t t;
		//srand((unsigned)time(&t)+10);

		int randIters = rand()%m;

		for(int i=0; i < randIters; i++)
		{
			int randInd = rand()%m;
			int randVal = rand()%(thisvar->constVal);

			// reads the shared value at index randInd into locVal
			fo.read(thid, randInd, locVal);
			
			readTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " reads from " << randInd << " a value " << locVal << " at time " <<  asctime(readTime) << endl;
			pthread_mutex_unlock(&pLock);

			// update the value
			locVal += randVal;

			// writes back to shared memory 
			fo.write(thid, randInd, locVal);
			writeTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " writes to " << randInd << " a value " << locVal << " at time " <<  asctime(writeTime) << endl;
			pthread_mutex_unlock(&pLock);

			int randTime = ex(thisvar->dur1);
			sleep(0.001 * randTime);
		}

		status = fo.tryC(thid);
		cout << "status is " << status << endl;

		commitTime = localtime(&current);

		pthread_mutex_lock(&pLock);
		cout << "Thread id " << thid << " Transaction " << tid << " tryCommits with result " << status << " at time " << asctime(commitTime) << endl;
		pthread_mutex_unlock(&pLock);

		abortCnt++;
		cout << "abort count is " << abortCnt << endl;

	}while(status != 1);

	gettimeofday(&timevar, NULL);
	long long critEndTime = timevar.tv_usec;
	txEndTime[thid] = critEndTime;

	//commitDelay[thid] = critEndTime - critStartTime;
	//abortCount[thid] = abortCnt;
	
	commit_answer +=  (critEndTime - critStartTime);
	abort_answer +=	 abortCnt;  
	
	return NULL;
}

struct arguments *data[max];		//argument structure array 

int main()
{

	int constVal, avg1;

	FILE *fp;
	fp = fopen("input.txt", "r");

	if (fp == NULL)
	{   
		printf("error in opening the file\n"); 
        exit(-1);
    }

	fscanf(fp, "%d %d %d %d", &n, &m, &constVal, &avg1);

	//sharedMem = (int*) malloc(sizeof(int) * m);
	sharedMem = new int[m];

	pthread_t nthread[n];

	buffer = new int*[n];
	readSet = new int*[n];
	writeSet = new int*[n];
	txStatus = new int[n];
	txStartTime = new long long int[n];
	txEndTime = new long long int[n];

	for(int i=0; i<n; i++)
	{
		data[i] =(struct arguments *) malloc (sizeof(struct arguments));
		data[i]->n = n;
		data[i]->m = m;
		data[i]->constVal = constVal;
		data[i]->dur1 = avg1;
		data[i]->cur = i;

		pthread_create(&nthread[i], NULL, updtMem, data[i]);		//create thread which executes on testCS
	}

	for (int i = 0; i < n; i++)
	{
		pthread_join(nthread[i], NULL);		//all threads join
	}

	cout << "average commit delay is " << abs(commit_answer/n) << " micro seconds " << endl;
	cout << "average number of aborts is " << (abort_answer/n) << endl;

	return 0;
}