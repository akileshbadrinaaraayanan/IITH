
#include <iostream>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <cmath>
#include <map>
#include <atomic> 
#include <unistd.h>
#include <time.h>
#include <pthread.h>
//#include <bits/stdc++.h>

using namespace std;
int n,m; // n- no. of threads m- no. of indices in the array 
int *arr; //shared array between threads
int lamda; // Mean of exponetial delay
random_device rd; // seeding for the gaussian random number generation
mt19937 gen(rd());
int constVal;

int **writeSet; //WriteSet for transactions
int **readSet;  //ReadSet for transactions
int **buffer;   //Local buffer used for defferred writes
int **conflict_set;	//Conflict set

int* committed; //To keep track of committed transactions
long* startTime; //To keep track of start time of transactions
long* endTime; 	//To keep track of end time of transactions
long* commitTime; // To keep track of commit delays to calculate the average commit delay later.
long* abortCount; // To keep trac of abort Counts to calculate the average abortCount
int* pre_commit_set; //Precommit set
int* statusThread;

pthread_mutex_t lock; //Lock used to ensure that val-write 
pthread_mutex_t lock2; //Lock used to ensure that print statements doen't mix up.
pthread_mutex_t glock;	//Lock for global vars
 

class RWV {
	volatile atomic<int> id; //atomic so that operations on id are atomic
	
public:
	RWV() //Constructor
	{   
		id=0; //id Initially 0
	}
	
	//Returns a new transaction id
	int begin_trans()
	{
		//startTime[id]=new long(0);
		id++;
		return (int) id;
	}

	//Reads at appropriate index 
	//Protected by same lock that is used in val-write  so that when the transaction
	int read(int id,int index)
	{
		pthread_mutex_lock(&glock);
		int l=arr[index];
		pthread_mutex_unlock(&glock);
		return l;
	}


	//Writes to a local buffer
	void write(int id,int index, int l)
	{
		pthread_mutex_lock(&glock);
		buffer[id][index]=l;
		pthread_mutex_unlock(&glock);
	
	}

	//Try commit checks the RWV condition for allowing a transaction to commit. If a transaction can commit
	// then we write the local buffer to the shared memory and return 1 indicating that the transaction can commit.
	int tryCommit(int id)
	{	
		// pthread_mutex_lock(&lock);
		//cout << "inside tryCommit \n";

		//Temporary allocations useful in validation
		long* endTime_temp = new long[n];
		long* startTime_temp = new long[n];

		int writeSet_temp[n][m];
		int readSet_temp[n][m];

		//Creating temporary copies of readSet, writeSet, endTime and startTime which is useful for checking condition in validation phase
		//without modifying the actual original values. 
		pthread_mutex_lock(&glock);
		for(int j=0; j<n; j++)
		{
			endTime_temp[j] = endTime[j];
			startTime_temp[j] = startTime[j]; 

			for(int k=0; k<m; k++)
			{
				writeSet_temp[j][k] = writeSet[j][k];
				readSet_temp[j][k] = readSet[j][k];
			}
		}
		pthread_mutex_unlock(&glock);

		//If the transaction being evaluated doesn't satisfy the conditions of RWV, commit is set to 0.
		for(int j=0;j<n;j++)
		{
			if(endTime_temp[j]==0 && startTime_temp[j]!=0 && j!=id)
			{
					for(int k=0; k<m; k++)
					{
						if(writeSet_temp[id][k]==readSet_temp[j][k] && readSet_temp[j][k]==1)
							{
								cout << "Thread "<<j<<" is going to be aborted "<<endl;
								pthread_mutex_lock(&glock);
								statusThread[j] = 0;
								conflict_set[j][k] = buffer[id][k]; 
								pthread_mutex_unlock(&glock);
							}
					}			
			}
		}
	
		// pthread_mutex_unlock(&lock);
		return 1;
	}

}rwv;


//Same as what was given in the question
void* updtMem(void *arg){
	int status =0; //0 for abort and 1 for commit
	int abortCnt=-1; // keeps track of abort count
	long critStartTime, critEndTime;
	critStartTime=time(0); // keep track of critical section start time
	
	bool firstRun = true;	//variable to keep track of whether it's the firstrun or not
	std::vector<int> thr_readSet;	//local readSet for each thread
	std::vector<int> thr_readValues;	//local readValue for each thread, 

	int *atid;
	atid = (int*) arg;
	int ttid=*atid;
	statusThread[ttid] = 2;		//status id is 2 => it's currently active.
	// ttid is thread is, id is transaction id
	cout << "thread id is " << ttid << "\n";

	//Allocating readSet, writeSet, buffer and conflictSet.
	normal_distribution<> distribution1(lamda,0.5);  //mean m1 and variance 2
	//cout << "successfully allocated 1\n";
	readSet[ttid]=new int[m];	
	//cout << "successfully allocated 2\n";
	writeSet[ttid]=new int[m];
	//cout << "successfully allocated 3\n";
	buffer[ttid]=new int[m];
	//cout << "successfully allocated 4\n";
	conflict_set[ttid] = new int[m];


	//cout << "successfully allocated \n";
	do{
		int id=rwv.begin_trans(); // begins a new transaction id
		startTime[ttid]=time(0);
		int randIters= rand()%m;  // gets the number of iterations to be updated
		
		// Only for the first run, randIters is random. If not, just make it equal to the size of thread readset
		if(firstRun != true)
		{
			randIters = thr_readSet.size();
		}

		//cout << "randIters is " << randIters << endl;

		for(int j=0;j<randIters;j++)
		{
			//get the next randIndex
			int randInd=rand()%m;
			int randVal=rand()%constVal; 
			int locVal;

			if(firstRun != true)
			{
				randInd = thr_readSet[j];
			}
			else //If it's the first run 
			{
				thr_readSet.push_back(randInd);
			}

			pthread_mutex_lock(&glock);
			int temp1 = statusThread[ttid];
			pthread_mutex_unlock(&glock);
			
			if(temp1 == 0)
				break;

			//Read a shared memory
			//If it's the firstRun
			if(firstRun == true)
			{
				locVal=rwv.read(ttid, randInd); 
				
				//Since readSet, conflict_set are global, lock should be used
				pthread_mutex_lock(&glock);
				readSet[ttid][randInd]=1;	
				conflict_set[ttid][randInd] = locVal;	//update conflict set to locVal
				pthread_mutex_unlock(&glock);
				
				thr_readValues.push_back(locVal);
			}
			else
			{
				locVal = thr_readValues[j];
				
				//Since readSet, conflict_set are global, lock should be used
				pthread_mutex_lock(&glock);
				readSet[ttid][randInd] = 1;
				conflict_set[ttid][randInd] = locVal;	//update conflict set to locVal
				pthread_mutex_unlock(&glock);
			}
			
			
			time_t readTime = time(0);
		
			//cout << "Before read print \n";
			//Print lock
			pthread_mutex_lock(&lock2);
			cout<<"Thread id "<< ttid <<" Transaction "<< id<< " reads from " <<randInd<<" a value "<<locVal<<" at time "<< gmtime(& readTime)->tm_hour<<":"<<gmtime(& readTime)->tm_min<<":"<<gmtime(& readTime)->tm_sec<<"\n";
			pthread_mutex_unlock(&lock2);
			locVal=locVal+randVal;
			rwv.write(ttid, randInd,locVal);
			
			pthread_mutex_lock(&glock);
				temp1 = statusThread[ttid];
			pthread_mutex_unlock(&glock);
			
			if(temp1 == 0)
				break;
			

			// request to write back to the shared memory
			pthread_mutex_lock(&glock);
			writeSet[ttid][randInd]=1;
			pthread_mutex_unlock(&glock);
			time_t writeTime = time(0);
			pthread_mutex_lock(&lock2);
			cout<<"Thread id "<< ttid <<" Transaction "<< id<< " writes to " <<randInd<<" a value "<<locVal<<" at time "<< gmtime(& writeTime)->tm_hour<<":"<<gmtime(& writeTime)->tm_min<<":"<<gmtime(& writeTime)->tm_sec<<"\n";
			pthread_mutex_unlock(&lock2);
			long randTime=round(distribution1(gen));
		
			// sleep for a random amount of time which simulates some complex computation
			sleep(randTime);
		}
		//cout << "after for loop\n";

		pthread_mutex_lock(&glock);
		pre_commit_set[ttid] = 1;
		// int temp = statusThread[ttid];
		pthread_mutex_unlock(&glock);

		pthread_mutex_lock(&lock);
		pthread_mutex_lock(&glock);
			int temp = statusThread[ttid];
		pthread_mutex_unlock(&glock);

		if(temp == 2)	//If the thread ttid is currently active
		{
			for(int k=0;k<m;k++)
				if(writeSet[ttid][k]==1)
					arr[k]=buffer[ttid][k];

			status=rwv.tryCommit(ttid); // ttid is thread is, id is transaction id
			committed[ttid]=status;
			statusThread[ttid] = 1;
			time_t cTime = time(0);
			pthread_mutex_lock(&lock2);
			cout<<"Thread id " << ttid << " Transaction "<< id<< " tryCommits with result "<< status <<" at time "<<gmtime(& cTime)->tm_hour<<":"<<gmtime(& cTime)->tm_min<<":"<<gmtime(& cTime)->tm_sec<<"\n";
			pthread_mutex_unlock(&lock2);

			endTime[ttid]=time(0);
		}
		else 			
		{
			for(int i=0;i<m;i++)
			{
				readSet[ttid][i]=0;
				writeSet[ttid][i]=0;
				buffer[ttid][i]=0;	
			}
			statusThread[ttid] = 2;		//Make it active
			startTime[ttid]=0;			//Reinitialize startTime
			cout<< "Thread " << ttid << " transaction "<< id<<" aborted \n";
			
			for(int k=0; k<thr_readSet.size(); k++)
				thr_readValues[k] = conflict_set[ttid][thr_readSet[k]];
		}
		pthread_mutex_unlock(&lock);
		abortCnt++;   // Increment the abort count
		
		cout << "value of status is " << status << endl;	
	}while(status!=1);

	critEndTime=time(0); // keep track of critical section end time
	commitTime[ttid]=critEndTime- critStartTime;
	abortCount[ttid]=abortCnt;
}
	

int main(int argc, char *argv[])
{
	FILE *fp;  
        //filename taken as command line argument and it is opened in read mode
    fp = fopen(argv[1],"r"); 
	
    fscanf(fp, "%d %d %d %d", &n, &m, &constVal, &lamda);
    //cin >> n >> m >> constVal >> lamda;

//Initialising all the pointers and double pointers
    //cout << "before allocation \n";

	writeSet = new int*[n];
	readSet = new int*[n];
	buffer = new int*[n];
	conflict_set = new int*[n];

	committed = new int[n];
	startTime = new long[n];
	endTime = new long[n];
	commitTime= new long[n];
	abortCount=new long[n];
	statusThread = new int[n];
	pre_commit_set = new int[n];

	arr= new int[m];

	//cout << "after allocation \n";

	//In the below section of code, we create n threads of updtMem and wait on them. 
	//Thread creation error is also handled.

	pthread_t threads[n];
	int td[n];
	
	for(int i=0;i<n;i++)
		td[i]=i;

	for(int i=0;i<m;i++)
		arr[i]=0;

	for(int i=0; i < n; i++)
	{
		int rc = pthread_create(&threads[i], NULL,
                          updtMem, (void *)&td[i]);
      	if (rc)
      	{
        	cout << "Error:Unable to create thread," << rc << endl;
         	exit(-1);
      	}
      	cout << "thread creating \n";
	}
	
	for(int i=0;i<n;i++)
	{
		pthread_join(threads[i],NULL);
	}

	//All the averaging is done below:

	double average = 0.0;
	double average_ac = 0.0;
	
	for(int i=0; i < n; i++)
	{
		average += commitTime[i];
		average_ac += abortCount[i];
	}

	average = average/n;
	average_ac = average_ac/n;
	cout<<"Average Commit Delay "<<average<<" secs"<<endl;
	cout<<"Average Abort Count "<<average_ac<<endl;
}
