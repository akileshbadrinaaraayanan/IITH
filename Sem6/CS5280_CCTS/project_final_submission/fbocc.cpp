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
#include <vector>

using namespace std;

int nrot,nut,nst,m; // n- no. of threads of different kinds m- no. of indices in the array 
int *arr; //shared array between threads
int lamda; // Mean of exponetial delay
random_device rd; // seeding for the gaussian random number generation
mt19937 gen(rd());
int constVal;
int history_count=20;

//ROT VARIABLES
int **rot_readSet;  //ReadSet for transactions
int* rot_committed; //To keep track of committed transactions
long* rot_startTime; //To keep track of start time of transactions
long* rot_endTime; 	//To keep track of end time of transactions
long* rot_commitTime; // To keep track of commit delays to calculate the average commit delay later.
long* rot_abortCount; // To keep trac of abort Counts to calculate the average abortCount

//UT Variables
int **ut_readSet;  //ReadSet for transactions
int **ut_writeSet;  //ReadSet for transactions
int **ut_buffer;  //ReadSet for transactions
int* ut_committed; //To keep track of committed transactions
long* ut_startTime; //To keep track of start time of transactions
long* ut_endTime; 	//To keep track of end time of transactions
long* ut_commitTime; // To keep track of commit delays to calculate the average commit delay later.
long* ut_abortCount; // To keep trac of abort Counts to calculate the average abortCount


// ST Variables
int **st_readSet;  //ReadSet for transactions
int **st_writeSet;  //ReadSet for transactions
int **st_buffer;  //ReadSet for transactions
int* st_committed; //To keep track of committed transactions
long* st_startTime; //To keep track of start time of transactions
long* st_endTime; 	//To keep track of end time of transactions
long* st_commitTime; // To keep track of commit delays to calculate the average commit delay later.
long* st_abortCount; // To keep trac of abort Counts to calculate the average abortCount

// Arrays which contain the restart details
int *st_restart; 
int *rot_restart;  
int *ut_restart; 


pthread_mutex_t lock; //Lock used to ensure that val-write 
pthread_mutex_t lock2; //Lock used to ensure that print statements doen't mix up.

//Broadcast variables
int* broadcast;
int* broadcast_buffer;
// To keep track of commit cycles
int* rot_commit_cycle;
int* ut_commit_cycle;
vector<int*> history_server;


volatile atomic<int> id; //Transaction id


class RotLock {
public:
	//Returns a new transaction id
	int begin_trans(){
		id++;
		return (int) id;
	}

	//Reads at appropriate index 
	//Protected by same lock that is used in val-write  so that when the transaction  
	int read(int id,int index){
		pthread_mutex_lock(&lock);
		int l=arr[index];
		pthread_mutex_unlock(&lock);
		return l;
	}
	//Implements the partial validate phase
	int partialValidate(int id){
		int status=1;
		for(int i=0;i<m;i++){
			if(broadcast[i+1]==rot_readSet[id][i]&&broadcast[i+1]==1)
				status=0;
		}
		if(status==1){
			rot_commit_cycle[id]=broadcast[0];
		}
		cout<<"\n";
		/*
		for(int i=0;i<=m;i++){
			cout<<broadcast[i];
		}
		*/
		cout<<"\n";
	    return status;
	}

}rot_lock;

class UtLock {
	
public:
	
	//Returns a new transaction id
	int begin_trans(){
		id++;
		return (int) id;

	}

	//Reads at appropriate index 
	//Protected by same lock that is used in val-write  so that when the transaction  
	int read(int id,int index){
		pthread_mutex_lock(&lock);
		int l=arr[index];
		pthread_mutex_unlock(&lock);
		return l;
	}
	//Writes to a local buffer
	void write(int id,int index, int l){
		pthread_mutex_unlock(&lock);
		ut_buffer[id][index]=l;
		pthread_mutex_unlock(&lock);
	
	}

	//Implements the partial validate phase
	int partialValidate(int id){
		int status=1;
		for(int i=0;i<m;i++){
			if(broadcast[i+1]==ut_readSet[id][i]&&broadcast[i+1]==1)
				status=0;
		}
		if(status==1){
			ut_commit_cycle[id]=broadcast[0];
		}
		
	    return status;
	}
		// Contains final validation and forward validation done at the server
		int tryCommit(int id){

			//cout<<"\n\n\nAMIGOS\n\n\n";
			int status=1;
			int intial_commit=ut_commit_cycle[id];
			int min=intial_commit+100;
			
			for(int i=0;i<history_server.size();i++){
				if(history_server[i][0]<min)
					min=history_server[i][0];
			}

			if(min>intial_commit){
				//cout<<"\nITS ME\n";
				return 0;
			}
			else{
				for(int i=0;i<history_server.size();i++){
					for(int j=1;j<=m;j++){
						if(ut_readSet[id][j-1]==history_server[i][j]&&history_server[i][j]==1&&history_server[i][0]>=min){
							//cout<<"\nITS ME BOSS\n";
							return 0;
						}
					}
				}
			}

			if(status==1){
				for(int i=0;i<nst;i++){
					if(st_endTime[i]==0&&st_startTime[i]!=0){
						for(int k=0;k<m;k++){
							if(ut_writeSet[id][k]==st_readSet[i][k]&&st_readSet[i][k]==1)
								st_restart[i]=1;
						}
					}

				}
				

			}

			if(status=1){
				//Copying local buffer to shared memory
				for(int k=0;k<m;k++)
					if(ut_writeSet[id][k]==1)
						arr[k]=ut_buffer[id][k];

		
				for(int i=0;i<m;i++){
					if(ut_writeSet[id][i]==1)
						broadcast_buffer[i+1]=1;
				}
			}
			return status;
		}
	
}ut_lock;

class StLock {
	
public:
	
	//Returns a new transaction id
	int begin_trans(){
		//startTime[id]=new long(0);
		id++;
		return (int) id;

	}

	//Reads at appropriate index 
	//Protected by same lock that is used in val-write  so that when the transaction
	int read(int id,int index){
		pthread_mutex_lock(&lock);
		int l=arr[index];
		pthread_mutex_unlock(&lock);
		return l;
	}


	//Writes to a local buffer
	void write(int id,int index, int l){
		pthread_mutex_unlock(&lock);
		st_buffer[id][index]=l;
		pthread_mutex_unlock(&lock);
	
	}

	//Try commit checks the FOCC condition for server transactions for allowing a transaction to commit. If a transaction can commit
	// then we write the local buffer to the shared memory and return 1 indicating that the transaction can commit.
	int tryCommit(int id){
		
		pthread_mutex_lock(&lock);
		int commit=1; 

		//If the transaction being evaluated doesn't satisfy the conditions of FOCC, commit is set to 0.
		for(int j=0;j<nst;j++){
			if(st_endTime[j]==0&&st_startTime[j]!=0&&j!=id){
					for(int k=0;k<m;k++){
						if(st_writeSet[id][k]==st_readSet[j][k]&&st_readSet[j][k]==1)
							st_restart[j]=1;
				}
			}
		}

		//Copying local buffer to shared memory
		if(commit==1){
			for(int k=0;k<m;k++)
				if(st_writeSet[id][k]==1)
					arr[k]=st_buffer[id][k];
			for(int i=0;i<m;i++){
				if(st_writeSet[id][i]==1)
					broadcast_buffer[i+1]=1;

			}
			broadcast_buffer[0]=::id;

			for(int i = 0; i <=m; ++i){
				broadcast[i]=broadcast_buffer[i];
			}

			int *temp= new int[m+1];
			for(int i = 0; i <=m; ++i){
				temp[i]=broadcast[i];
			}
			history_server.push_back(temp);

		}
		pthread_mutex_unlock(&lock);
		return commit;
	}

}st_lock;

 // Application for read only transactions
void* rot(void *arg){
	int status =0; //0 for abort and 1 for commit
	int abortCnt=-1; // keeps track of abort count
	long critStartTime, critEndTime;
	critStartTime=time(0); // keep track of critical section start time
	
	int *atid;
	atid = (int*) arg;
	int ttid=*atid;

	normal_distribution<> distribution1(lamda-1.4,0.3);  //mean m1 and variance 2
	rot_readSet[ttid]=new int[m];
	
	do{
		int id=rot_lock.begin_trans(); // begins a new transaction id
		rot_startTime[ttid]=time(0);
		int nor= rand()%m;
		for(int i=0;i<nor;i++){
			int temp=rand()%m;
			rot_readSet[ttid][temp]=1;
		}

		status=rot_lock.partialValidate(ttid);
		if(status==1){

			for(int i=0;i<m;i++){
				if(rot_restart[ttid]==1)
					goto restart_rot;
				if(rot_readSet[ttid][i]==1){

					int locVal;
					//Read a shared memory
					locVal=rot_lock.read(ttid,i);

					time_t readTime = time(0);
					//Print Lock
					pthread_mutex_lock(&lock2);
					cout<<"Read Only Thread "<< ttid <<" Transaction "<< id<< " reads from " <<i<<" a value "<<locVal<<" at time "<< gmtime(& readTime)->tm_hour<<":"<<gmtime(& readTime)->tm_min<<":"<<gmtime(& readTime)->tm_sec<<"\n";
					pthread_mutex_unlock(&lock2);
					

				}
				long randTime=round(distribution1(gen));
				sleep(randTime);
			}
			
		}
		rot_committed[ttid]=status;
		restart_rot:
		time_t cTime = time(0);
		
		pthread_mutex_lock(&lock2); 
		cout<<"Transaction "<< id<< " tryCommits with result "<< status <<" at time "<<gmtime(& cTime)->tm_hour<<":"<<gmtime(& cTime)->tm_min<<":"<<gmtime(& cTime)->tm_sec<<"\n";
		pthread_mutex_unlock(&lock2);
		abortCnt++;   // Increment the abort count
		if(status==1){
			rot_endTime[ttid]=time(0);
		}
		if(status==0){
			for(int i=0;i<m;i++){
				rot_readSet[ttid][i]=0;
				
			}
			rot_startTime[ttid]=0;
			rot_restart[ttid]=0;
		}
	}while(status!=1);
	critEndTime=time(0); // keep track of critical section end time
	rot_commitTime[ttid]=critEndTime- critStartTime; 
	rot_abortCount[ttid]=abortCnt;

}

// Application for Update transactions
void* ut(void *arg){
	int status =0; //0 for abort and 1 for commit
	int abortCnt=-1; // keeps track of abort count
	long critStartTime, critEndTime;
	critStartTime=time(0); // keep track of critical section start time
	
	int *atid;
	atid = (int*) arg;
	int ttid=*atid;

	normal_distribution<> distribution1(lamda,0.5);  //mean m1 and variance 2
	ut_readSet[ttid]=new int[m];
	ut_writeSet[ttid]=new int[m];
	ut_buffer[ttid]=new int[m];
	
	do{
		int id=ut_lock.begin_trans(); // begins a new transaction id
		ut_startTime[ttid]=time(0);
		int nor= rand()%m;
		for(int i=0;i<nor;i++){
			int temp=rand()%m;
			ut_readSet[ttid][temp]=1;
			ut_writeSet[ttid][temp]=1;
		}

		status=ut_lock.partialValidate(ttid);
		if(status==1){

			for(int i=0;i<m;i++){
				if(ut_restart[ttid]==1){
					cout<<"\n\n\nHOLA HOLA HOLA\n\n\n";
					goto restart_ut;

				}
				if(ut_readSet[ttid][i]==1){

					int locVal;
					//Read a shared memory
					locVal=ut_lock.read(ttid,i);
					
					time_t readTime = time(0);
					//Print Lock
					pthread_mutex_lock(&lock2);
					cout<<"Update Thread "<< ttid <<" Transaction "<< id<< " reads from " <<i<<" a value "<<locVal<<" at time "<< gmtime(& readTime)->tm_hour<<":"<<gmtime(& readTime)->tm_min<<":"<<gmtime(& readTime)->tm_sec<<"\n";
					pthread_mutex_unlock(&lock2);
					int randVal=rand()%constVal; 
					locVal=locVal+randVal;
					ut_lock.write(ttid,i,locVal);
					// request to write back to the shared memory
					time_t writeTime = time(0);
					pthread_mutex_lock(&lock2);
					cout<<"Update Thread "<< ttid <<" Transaction "<< id<< " writes to " <<i<<" a value "<<locVal<<" at time "<< gmtime(& writeTime)->tm_hour<<":"<<gmtime(& writeTime)->tm_min<<":"<<gmtime(& writeTime)->tm_sec<<"\n";
					pthread_mutex_unlock(&lock2);
					
					
					

				}
				long randTime=round(distribution1(gen));
				sleep(randTime);
				
			}
		}
		
		status=ut_lock.tryCommit(ttid); // try to commit the transaction
		restart_ut:
		ut_committed[ttid]=status;
		
		time_t cTime = time(0);
		
		pthread_mutex_lock(&lock2); 
		cout<<"Update Transaction "<< id<< " tryCommits with result "<< status <<" at time "<<gmtime(& cTime)->tm_hour<<":"<<gmtime(& cTime)->tm_min<<":"<<gmtime(& cTime)->tm_sec<<"\n";
		pthread_mutex_unlock(&lock2);
		abortCnt++;   // Increment the abort count
		if(status==1){
			ut_endTime[ttid]=time(0);
		}
		if(status==0){
			for(int i=0;i<m;i++){
				ut_readSet[ttid][i]=0;
				ut_writeSet[ttid][i]=0;
				ut_buffer[ttid][i]=0;
				
			}
			ut_startTime[ttid]=0;
			ut_restart[ttid]=0;
		}
	}while(status!=1);
	critEndTime=time(0); // keep track of critical section end time
	ut_commitTime[ttid]=critEndTime- critStartTime; 
	ut_abortCount[ttid]=abortCnt;

}

//Application for Server Transactions

void* st(void* arg){
	int status =0; //0 for abort and 1 for commit
	int abortCnt=-1; // keeps track of abort count
	long critStartTime, critEndTime;
	critStartTime=time(0); // keep track of critical section start time
	
	int *atid;
	atid = (int*) arg;
	int ttid=*atid;

	normal_distribution<> distribution1(lamda,0.5);  //mean m1 and variance 2
	st_readSet[ttid]=new int[m];
	st_writeSet[ttid]=new int[m];
	st_buffer[ttid]=new int[m];

	do{
		int id=st_lock.begin_trans(); // begins a new transaction id
		st_startTime[ttid]=time(0);
		int randIters= rand()%m;  // gets the number of iterations to be updated
		
			
		for(int j=0;j<randIters;j++){
			//get the next randIndex
			if(st_restart[ttid]==1)
					goto restart_st;
			int randInd=rand()%m;
			int randVal=rand()%constVal; 
			int locVal;
			//Read a shared memory
			locVal=rot_lock.read(ttid, randInd); 
			st_readSet[ttid][randInd]=1;
			
			time_t readTime = time(0);
			//Print Lock
			pthread_mutex_lock(&lock2);
			cout<<"Server Thread "<< ttid <<" Transaction "<< id<< " reads from " <<randInd<<" a value "<<locVal<<" at time "<< gmtime(&readTime)->tm_hour<<":"<<gmtime(& readTime)->tm_min<<":"<<gmtime(& readTime)->tm_sec<<"\n";
			pthread_mutex_unlock(&lock2);
			locVal=locVal+randVal;
			st_lock.write(ttid, randInd,locVal);
			// request to write back to the shared memory
			st_writeSet[ttid][randInd]=1;
			time_t writeTime = time(0);
			pthread_mutex_lock(&lock2);
			cout<<"Server Thread "<< ttid <<" Transaction "<< id<< " writes to " <<randInd<<" a value "<<locVal<<" at time "<< gmtime(& writeTime)->tm_hour<<":"<<gmtime(& writeTime)->tm_min<<":"<<gmtime(& writeTime)->tm_sec<<"\n";
			pthread_mutex_unlock(&lock2);
			long randTime=round(distribution1(gen));
			//cout<<randTime;
			// sleep for a random amount of time which simulates some complex computation
			sleep(randTime);
		}

		status=st_lock.tryCommit(ttid); // try to commit the transaction
		st_committed[ttid]=status;
		restart_st:
		time_t cTime = time(0);
		
		pthread_mutex_lock(&lock2);
		cout<<"Transaction "<< id<< " tryCommits with result "<< status <<" at time "<<gmtime(& cTime)->tm_hour<<":"<<gmtime(& cTime)->tm_min<<":"<<gmtime(& cTime)->tm_sec<<"\n";
		pthread_mutex_unlock(&lock2);
		
		abortCnt++;    // Increment the abort count
		if(status==1){
			//endTime[id]=new long(0);
			st_endTime[ttid]=time(0);
		}
		if(status==0){
			for(int i=0;i<m;i++){
				st_readSet[ttid][i]=0;
				st_writeSet[ttid][i]=0;
				st_buffer[ttid][i]=0;
			}
			st_startTime[ttid]=0;
			st_restart[ttid]=0;
		}
		

			
	}while(status!=1);
	critEndTime=time(0); // keep track of critical section end time
	st_commitTime[ttid]=critEndTime- critStartTime;
	st_abortCount[ttid]=abortCnt;


}

int main(int argc, char *argv[]){
	
	//nst=5;
	//nrot=50;
	//nut=50;
	//m=30;
	//constVal=100;
	//lamda=2;

	FILE *fp;  
        //filename taken as command line argument and it is opened in read mode
    fp = fopen(argv[1],"r"); 
	
    fscanf(fp, "%d %d %d %d %d %d", &nst, &nrot, &nut, &m, &constVal, &lamda);


//Creation of memory for all variables
	st_writeSet = new int*[nst];
	st_readSet = new int*[nst];
	st_buffer = new int*[nst];
	st_committed = new int[nst];
	st_startTime = new long[nst];
	st_endTime = new long[nst];
	st_commitTime= new long[nst];
	st_abortCount=new long[nst];


	ut_writeSet = new int*[nut];
	ut_readSet = new int*[nut];
	ut_buffer = new int*[nut];
	ut_committed = new int[nut];
	ut_startTime = new long[nut];
	ut_endTime = new long[nut];
	ut_commitTime= new long[nut];
	ut_abortCount=new long[nut];


	
	rot_readSet = new int*[nrot];
	rot_committed = new int[nrot];
	rot_startTime = new long[nrot];
	rot_endTime = new long[nrot];
	rot_commitTime= new long[nrot];
	rot_abortCount=new long[nrot];

	st_restart=new int[nst];  
	rot_restart=new int[nrot];  
	ut_restart=new int[nut]; 

	broadcast=new int[m+1];
    broadcast_buffer=new int[m+1];
	rot_commit_cycle=new int[nrot];
	ut_commit_cycle=new int[nut];

	arr= new int[m];
//Definition of threads
	pthread_t threads_rot[nrot];
	int td_rot[nrot];

	pthread_t threads_ut[nut];
	int td_ut[nut];

	pthread_t threads_st[nst];
	int td_st[nst];

	for(int i=0;i<m;i++)
		arr[i]=0;

	for(int i=0;i<nst;i++)
		td_st[i]=i;

//Creation of different threads
	for( int i=0; i < nst; i++ ){
		int rc = pthread_create(&threads_st[i], NULL,
                          st, (void *)&td_st[i]);
      	if (rc){
        	cout << "Error:Unable to create thread," << rc << endl;
         	exit(-1);
      	}
	}

	sleep(3);

	for(int i=0;i<nrot;i++)
		td_rot[i]=i;


	for( int i=0; i < nrot; i++ ){
		int rc = pthread_create(&threads_rot[i], NULL,
                          rot, (void *)&td_rot[i]);
      	if (rc){
        	cout << "Error:Unable to create thread," << rc << endl;
         	exit(-1);
      	}
	}

	
	for(int i=0;i<nut;i++)
		td_ut[i]=i;

	for( int i=0; i < nut; i++ ){
		int rc = pthread_create(&threads_ut[i], NULL,
                          ut, (void *)&td_ut[i]);
      	if (rc){
        	cout << "Error:Unable to create thread," << rc << endl;
         	exit(-1);
      	}
	}

	
	
//Waiting for all the threads
	for(int i=0;i<nst;i++){
		pthread_join(threads_st[i],NULL);
	}

	for(int i=0;i<nrot;i++){
		pthread_join(threads_rot[i],NULL);
	}

	
	for(int i=0;i<nut;i++){
		pthread_join(threads_ut[i],NULL);
	}


	

	//All the averaging is done below:

	double rot_average=0.0;
	double rot_average_ac=0.0;
	for(int i=0;i<nrot;i++){
		rot_average+=rot_commitTime[i];
		rot_average_ac+=rot_abortCount[i];

	}
	rot_average=rot_average/nrot;
	rot_average_ac=rot_average_ac/nrot;
	cout<<"Average ROT Commit Delay "<<rot_average<<" secs"<<endl;
	cout<<"Average ROT Abort Count "<<rot_average_ac<<endl;

	//All the averaging is done below:
	double ut_average=0.0;
	double ut_average_ac=0.0;
	for(int i=0;i<nut;i++){
		ut_average+=ut_commitTime[i];
		ut_average_ac+=ut_abortCount[i];

	}
	ut_average=ut_average/nut;
	ut_average_ac=ut_average_ac/nut;
	
	cout<<"Average UT Commit Delay "<<ut_average<<" secs"<<endl;
	cout<<"Average UT Abort Count "<<ut_average_ac<<endl;


	double st_average=0.0;
	double st_average_ac=0.0;
	for(int i=0;i<nst;i++){
		st_average+=st_commitTime[i];
		st_average_ac+=st_abortCount[i];

	}
	st_average=st_average/nst;
	st_average_ac=st_average_ac/nst;
	cout<<"Average ST Commit Delay "<<st_average<<" secs"<<endl;
	cout<<"Average ST Abort Count "<<st_average_ac<<endl;



	return 0;
}
