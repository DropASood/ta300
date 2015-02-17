// CMPT-300 Assignment 2
// Part I: Measuring Context Switch Costs
// Archit Sood 301188293
// Tony Dinh 301142069

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/syscall.h> 

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

void barefunction(){}

unsigned long long loopCost(int iterations){
	struct timespec start; 		//variable that stores start time
	struct timespec stop;		//variable that stores end time
	unsigned long long result; 	//64 bit integer

	clock_gettime(CLOCK_REALTIME, &start);  //record wall-clock start time
	for(int i = 0 ; i < iterations; i++);
	clock_gettime(CLOCK_REALTIME, &stop);	//record wall-clock end time

	result = timespecDiff(&stop,&start);
	return result;

}


/*int main(){

//INITIALIZATION

	struct timespec start; 		//variable that stores start time
	struct timespec stop;		//variable that stores end time
	unsigned long long result; 	//64 bit integer

//CLOCK_REALTIME

	clock_gettime(CLOCK_REALTIME, &start);  //record wall-clock start time
	sleep(1);
	clock_gettime(CLOCK_REALTIME, &stop);	//record wall-clock end time

	result=timespecDiff(&stop,&start);		//compute difference

	printf("CLOCK_REALTIME Measured: %llu\n",result); //display result

//CLOCK_MONOTONIC

	clock_gettime(CLOCK_MONOTONIC, &start); //record monotonic start time
	sleep(1);
	clock_gettime(CLOCK_MONOTONIC, &stop);	//record monotonic end time

	result=timespecDiff(&stop,&start);		//compute difference

	printf("CLOCK_MONOTONIC Measured: %llu\n",result); //display result

//CLOCK_PROCESS_CPUTIME_ID
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);	//record process start time
	sleep(1);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);		//record process end time

	result=timespecDiff(&stop,&start);					//compute difference

	printf("CLOCK_PROCESS_CPUTIME_ID Measured: %llu\n",result); //display result

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);		//record thread start time
	sleep(1);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);		//record thread end time

	result=timespecDiff(&stop,&start);					//compute difference

	printf("CLOCK_THREAD_CPUTIME_ID Measured: %llu\n",result);	//display result

//MEASURE BARE FUNCTION
	clock_gettime(CLOCK_MONOTONIC, &start); //record monotonic start time
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);	//record monotonic end time

	result=timespecDiff(&stop,&start);		//compute difference

	printf("Bare Function Measured: %llu\n",result); //display result




return 0;
}
*/

//PART 2=======================================================================
int main(){
	struct timespec start; 		//variable that stores start time
	struct timespec stop;		//variable that stores end time
	struct timespec start1; 		//variable that stores start1 time
	unsigned long long result; 	//64 bit integer
	
	//Warming up the start and stop
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &start1);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured Overhead start stop cost: %llu\n",result);
	
	//Start and stop after caching:
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &start1);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured Overhead start stop cost after caching 1: %llu\n",result);
	
	//Start and stop after caching 2:
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &start1);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured Overhead start stop cost after caching 2: %llu\n",result);
	
	//Start and stop after caching 3:
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &start1);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured Overhead start stop cost after caching 3: %llu\n",result);
	
	//First function call before it is cached
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured: %llu\n",result);
	
	//2nd call
	//cost after caching
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured Cached 2nd time: %llu\n",result);
	
	//3rd call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 3rd time: %llu\n",result);
	
	//4th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 4th time: %llu\n",result);
	
	//5th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 5th time: %llu\n",result);
	
	//6th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 6th time: %llu\n",result);
	
	//7th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 7th time: %llu\n",result);
	
	//8th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 8th time: %llu\n",result);
	
	//9th call
	clock_gettime(CLOCK_MONOTONIC, &start);
	barefunction();
	clock_gettime(CLOCK_MONOTONIC, &stop);
	
	result=timespecDiff(&stop,&start);
	printf("CLOCK_MONOTONIC Measured 9th time: %llu\n",result);
	
	
	//unsigned int iteration = 1000000000;

	/* clock_gettime(CLOCK_REALTIME, &start); //record monotonic start time
		
	for(int i = 0 ; i < iteration ; i ++){
		barefunction();
	}
	
	clock_gettime(CLOCK_REALTIME, &stop);	//record monotonic end time

	result=timespecDiff(&stop,&start);		//compute difference

	unsigned long long bareCost = result - loopCost(iteration);
	float average = bareCost/(float)iteration;

	printf("Minimal Cost of a Function Call: %f ns/call\n",average); //display result */

//PART 3======================================================================
	/* clock_gettime(CLOCK_REALTIME, &start); //record monotonic start time
		
	for(int i = 0 ; i < iteration ; i ++){
		getpid();
	}
	
	clock_gettime(CLOCK_REALTIME, &stop);	//record monotonic end time

	result = timespecDiff(&stop,&start);		//compute difference

	unsigned long long systemCost = result - loopCost(iteration);
	average = systemCost/(float)iteration;

	printf("Minimal Cost per System Call: %f ns/call\n",average); //display result */

	return 0;
}

// PART 4.=====================================================================
/*int main(){
	pid_t pid;
	char* arg[2];
	arg[0] = "ls";
	arg[1] = NULL;
	
	char* arg2[3];
	arg2[0] = "grep";
	arg2[1] = "a";
	arg2[2] = NULL;
	
	char* arg3[3]; 
	arg3[0] = "grep";
	arg3[1] = "e";
	arg3[2] = NULL;


	int fd[4]; 	//pipes[0]: read-end for pipe 1
				//pipes[1]: write-end for pipe 1
				//pipes[2]: read-end for pipe 2
				//pipes[3]: write-end for pipe 2

//Initiate 2 Pipes===========================
	pipe(fd);	//Pipe from parent to child
	pipe(fd+2); //Pipe from child to parent

	pid = fork();

	if(pid == 0){ //child
		dup2(fd[1],STDOUT_FILENO);
		close(fd[0]); //?
		close(fd[1]);
		close(fd[2]);
		close(fd[3]);
		
		execvp(arg[0],arg);
	}

	else if (pid<0 ){
		perror("Forking");
		exit(1);
	}

	else{ //parent
		pid = fork();

		if(pid == 0){ //child
			dup2(fd[0],STDIN_FILENO);
			dup2(fd[3],STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			close(fd[2]);
			close(fd[3]);

			execvp(arg2[0],arg2);
		}

		else if (pid<0 ){
			perror("Forking");
			exit(1);
		}

		else{ // parent
			pid = fork();
			if(pid == 0){ //child
				dup2(fd[2],STDIN_FILENO);
				close(fd[0]); 
				close(fd[1]);
				close(fd[2]);
				close(fd[3]);

				execvp(arg3[0],arg3);
			}

			else if (pid<0 ){
				perror("Forking");
				exit(1);
			}
		
		}

	}

	close(fd[0]); 
	close(fd[1]);
	close(fd[2]);
	close(fd[3]);

	wait(0);
	wait(0);

	return 0;
}
*/


//================================================================================
/*
int main(){
	pid_t pid;

	int parent2child[2], child2parent[2];
	pipe(parent2child);
	pipe(child2parent);

	if((pid=fork()) == 0){
		close(parent2child[1]);
		close(child2parent[0]);
	}
	return 0;
}

*/















/*	
CLOCK_REALTIME:
	"It is the system time used for human time display, so it's 
	subject to corrections whenever the system checks a time server 
	or the system administrator updates the clock. If such adjustments
	are made during benchmarking, then the elapsed time between two 
	queries of CLOCK_REALTIME will be wrong. One of the other clocks 
	should be used instead, if available" [1].

	We would use CLOCK_REALTIME for benchmarking if we know that there 
	will not be any changes to the system real-time clock during the test
	and if we are required to provide human friendly time stamps in a 
	report (year/month/day).

CLOCK_MONOTONIC:
	"It reports an unsettable time that counts upwards starting at an 
	unspecified time zero in the past that's typically the system's boot 
	time. The time from CLOCK_MONOTONIC cannot be mapped to the current 
	real-world date and time, but it's the right tool for benchmarking 
	where we only need to find the elapsed time of an algorithm." [1]

	We would use CLOCK_MONOTONIC for benchmarking if we only care about the
	elapsed time for what we are measuring. However, it may not be accurate in
	measuring the absolute time that is spent on executing the process. [2]

CLOCK_PROCESS_CPUTIME_ID:
	It allows for the measure of the time taken by the process itself rather
	than the elapsed time of the system. However, due to some hardware having 
	seperate timers on each CPU, the result may be invalid if the process is 
	moved between CPUs. [2]

	We would use CLOCK_PROCESS_CPUTIME_ID when we wish to measure the total time
	taken by a process and can guarantee that the process will not switch CPUs
	during exection.

CLOCK_THREAD_CPUTIME_ID:
	It allows for the measure of the time taken by a thread itself rather than 
	the elapsed time of the system.

	We would use CLOCK_THREAD_CPUTIME_ID when our program contains multiple
	threads and we wish to measure the time taken by a CPU thread and can 
	guarantee that the process will not switch CPUs during exection.

References: 

[1] http://nadeausoftware.com/articles/2012/04/c_c_tip_how_measure_elapsed_real_time_benchmarking
[2] https://upvoid.com/devblog/2014/05/linux-timers/
*/ 