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

//Structure to pass arguments to function
struct arguments
{
	int cur;
	int n, m, constVal, dur1;
};

//Shared memory variables
int *sharedMem, m, n;

//Lock mutex
pthread_mutex_t lock;
pthread_mutex_t pLock;
struct timeval timevar;

//Buffer, readset and writeset for every transaction

//int *abortCount = new int[n];
int abort_answer = 0;

//long long int *commitDelay = new long long int[n]; 
long long int commit_answer = 0;

//Variables buffer, readSet, writeset, txStatus
//readSet, writeSet and buffer are 2D arrays because they are needed for every transaction
int** buffer;
long long int* max_r_scheduled;
long long int* max_w_scheduled;
int** writeSet;
int *txStatus;
long long int *txStartTime;
long long int *txEndTime;

//Main class for BTO
class BTO
{
	volatile atomic<int> id;

public:
	BTO()
	{
		id = 0;	//Constructor
	}

	int begin_trans();	

	//i is the id in all these cases
	void read(int i, int x, int &l);
	void write(int i, int x, int l);
	int tryC(int id, int type, int rndId);
};

//Assign transaction id in an incremental fashion
int BTO::begin_trans()	
	{
		id++;
		return id;
	}

//Read function, notice the lock
void BTO::read(int id, int x, int &l)
	{
		pthread_mutex_lock(&lock);
			l = sharedMem[x];
		pthread_mutex_unlock(&lock);
		//readSet[id][x] = 1;	
	}

//Write, we write to to the buffer initially
void BTO::write(int id, int x, int l)
	{
		//pthread_mutex_unlock(&lock);
		buffer[id][x] = l;
		//pthread_mutex_unlock(&lock);

		writeSet[id][x] = 1;
	}

//Try commit function
//I do A U B => A bar intersection B bar
int BTO::tryC(int id, int type, int rndId)
{
	int toAbort = 0;
	//Notice the lock
	pthread_mutex_lock(&lock);

	if(type == 0)
	{
		if(txStartTime[id] < max_w_scheduled[rndId])
		{
			toAbort = 1;
		}
		else if(txStartTime[id] > max_r_scheduled[rndId])
		{
			max_r_scheduled[rndId] = txStartTime[id];
		}
	}
	else if(type == 1)
	{
		if(txStartTime[id] < max_r_scheduled[rndId] || txStartTime[id] < max_w_scheduled[rndId])
		{
			toAbort = 1;
		}
		else if(txStartTime[id] > max_w_scheduled[rndId])
		{
			max_w_scheduled[rndId] = txStartTime[id];
		}
	}

	if(toAbort == 0)
	{
		txStatus[id] = 1;	//Commit

		//Perform deferred writes
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
		txStatus[id] = -1;	//Abort
		//return -1;
	}
	
	pthread_mutex_unlock(&lock);
	return txStatus[id];
}

BTO bo;

void *updtMem(void *curvar)
{
	struct arguments *thisvar = (struct arguments *) curvar;	

// Reqtime, actTime and exitTime structures
	struct tm *readTime, *writeTime, *commitTime;

	int status = -1;       //  -1 => ABORT
	int abortCnt = -1;		//keeps track of abort count
	int m = thisvar->m;
	int thid = thisvar->cur;

	//To get time
	time_t current;
	time(&current);

	//critStartTime -> time when it enters the critical section
	gettimeofday(&timevar, NULL);
	long long critStartTime = timevar.tv_usec;
	
	//FOCC fo;
	pthread_mutex_lock(&pLock);
	cout << "thread id is " << thid << endl;
	pthread_mutex_unlock(&pLock);

	//Intialize these arrays
	//There are m data items
	buffer[thid] = new int[m];
	writeSet[thid] = new int[m];
	
	//txStatus[thid] = 0;			//Tx is live
	//status = 0;
	do
	{
		txStartTime[thid] = 0;
		//txEndTime[thid] = 0;

		txStatus[thid] = 0;

		for(int j=0; j<m; j++)
		{
			buffer[thid][j] = 0;
			writeSet[thid][j] = 0;
		}
		//Get transaction id
		int tid = bo.begin_trans();

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
			bo.read(thid, randInd, locVal);	
			readTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " reads from " << randInd << " a value " << locVal << " at time " <<  asctime(readTime) << endl;
			pthread_mutex_unlock(&pLock);
			
			status = bo.tryC(thid, 0, randInd);
			cout << "status is " << status << endl;
			commitTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " tryCommits with result " << status << " at time " << asctime(commitTime) << endl;
			pthread_mutex_unlock(&pLock);

			abortCnt++;
			cout << "abort count is " << abortCnt << endl;

			

			// update the value
			locVal += randVal;

			// writes back to shared memory 
			bo.write(thid, randInd, locVal);
			writeTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " writes to " << randInd << " a value " << locVal << " at time " <<  asctime(writeTime) << endl;
			pthread_mutex_unlock(&pLock);

			status = bo.tryC(thid, 1, randInd);
			cout << "status is " << status << endl;
			commitTime = localtime(&current);

			pthread_mutex_lock(&pLock);
			cout << "Thread id " << thid << " Transaction " << tid << " tryCommits with result " << status << " at time " << asctime(commitTime) << endl;
			pthread_mutex_unlock(&pLock);

			abortCnt++;
			cout << "abort count is " << abortCnt << endl;

			int randTime = ex(thisvar->dur1);
			sleep(0.001 * randTime);
		}

	}while(status != 1);

	gettimeofday(&timevar, NULL);
	long long critEndTime = timevar.tv_usec;
	txEndTime[thid] = critEndTime;

	//commitDelay[thid] = critEndTime - critStartTime;
	//abortCount[thid] = abortCnt;
	
	//Get commit time and abort time
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
	max_r_scheduled = new long long int[m];
	max_w_scheduled = new long long int[m];

	//Create n threads
	pthread_t nthread[n];

	buffer = new int*[n];
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