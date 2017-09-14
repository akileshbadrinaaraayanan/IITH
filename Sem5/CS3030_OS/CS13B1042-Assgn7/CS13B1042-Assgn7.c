// Implemented by Akilesh B, cs13b1042
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
	/* Setting process to run on only one core */
	cpu_set_t mask;		
	CPU_ZERO(&mask);	
	CPU_SET(0, &mask);
	int result;

	result = sched_setaffinity(0, sizeof(mask), &mask);
	
	//if result is -1, cpu affinity not set properly.
	if(result == -1)
	{
		printf("error setting cpu affinity");
	}
	
	int n,k;

	//Open file and read input from file
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	
	int input[n];
	char inp[n][15];	//more than enough

	 for(k=0; k<n; k++)
	{
		fscanf(fp, "%d %s", &input[k], inp[k]);
	}
	// pid of children
	// id of children
	pid_t pids[n];
	char unique[10];

for(k=0; k<n; k++)
{	//create 10 children processes.
	if((pids[k] = fork()) < 0)
	{
		perror("fork error");
		abort();
	}
	else if(pids[k] == 0)
	{
	//child process
	// Setting affinity for child processes
	cpu_set_t mask1;
	CPU_ZERO(&mask1);
	CPU_SET(0, &mask1);
	int result1;

	result1 = sched_setaffinity(0, sizeof(mask1), &mask1);
	
	if(result1 == -1)
	{
		//error if result is -1
		printf("error setting cpu affinity");
	}
		struct sched_param param;
		
		// set priority as maximum
		if(strcmp(inp[k], "HIGHEST") == 0)
		{
			//get max priority of the given scheduling algo
			param.sched_priority = sched_get_priority_max(input[k]);
		}
		// set priority as minimum
		else if(strcmp(inp[k], "LOWEST") == 0)
		{
			//get min priority of given scheduling algo
			param.sched_priority = sched_get_priority_min(input[k]);	
		}
		// directly convert string into integer
		else
		{
			param.sched_priority = atoi(inp[k]);
		}
		// if 0 => FIFO
		 if(input[k] == 0)
		{
			sched_setscheduler(pids[k], SCHED_FIFO, &param);
		}
		//If 1 => RR
		else if(input[k] == 1)
		{
			sched_setscheduler(pids[k], SCHED_RR, &param);	
		}
		//If 2 => OTHER
		else if(input[k] == 2)
		{		
			sched_setscheduler(pids[k], SCHED_OTHER, &param);	
		}
		
		struct timeval timevar;
		gettimeofday(&timevar, NULL);
		long long t1 = timevar.tv_usec;
		//printf("process %d time %lld\n", k, t1);

		sprintf(unique, "id[%d]", k);
		//setting the arguments of the longrun.c file
		//longrun.c is compiled to produce long.o output file
		char *argv[] = {"long", unique, "1000000", "10", NULL};
		execvp("./long", argv);
		
		exit(0);
	}
}

int status;
pid_t pid;

// wait for all children to finish
	for(k=0; k<n; k++)
	{
		pid = wait(&status);
	}
	fclose(fp);	
}