// CMPT-300 Assignment 2
// Part I: Measuring Context Switch Costs
// Archit Sood 301188293
// Tony Dinh 301142069

/*=============
   PROBLEM 4
===============*/

#define _GNU_SOURCE
#include <sched.h>
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

struct timespec start; 			//variable that stores start time
struct timespec stop;			//variable that stores end time
signed long long int result; 	//64 bit integer

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

// Puts clock_gettime() and sched_yield() into cache memory
void warmup(){
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	sched_yield();
}

// Measures an average overhead cost of clock_gettime(CLOCK_MONOTONIC, &start) & clock_gettime(CLOCK_MONOTONIC, &stop)
double timeCost(){
	signed long long int sum = 0;

	for(int i = 0 ; i < 100 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}

	double average = sum/100.0;
	return average;
}

// Measures an average overhead cost of reading 1-byte
double readAverage(){
	char r;
	int fd[1]; char c = '!';
	result = 0;
	signed long long int sum = 0;
	for(int i = 0 ; i < 1000 ; i++){
		write(fd[0], &c, 1);
		clock_gettime(CLOCK_MONOTONIC, &start);
		read(fd[0], &r, 1 );
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result- timeCost();
	}
	
	return sum/1000.0;
}

// Measures an average overhead cost of writing 1-byte
double writeAverage(){
	char c = '\0';
	result = 0;
	signed long long int sum = 0;
	for(int i = 0 ; i < 1000 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		write(1, &c, 1 );
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result - timeCost();
	}
	
	return sum/1000.0;
}

// Measures an average overhead cost of calling sched_yield()
double yieldAverage(){
	signed long long int sum = 0;
	for(int i = 0 ; i < 1000 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		sched_yield();
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result - timeCost();
	}
	
	return sum/1000.0;
}

int main(){
//SETTING PARENT AFFINITY==========================================
	cpu_set_t set;                                     // Define a set of CPUs
    CPU_ZERO (&set);                                   // Clear the set so there is no CPUs
    CPU_SET (1, &set);                                 // Add a core to the set
    sched_setaffinity(0, sizeof(cpu_set_t), &set);     // Set affinity of this process to the defined mask
  
 	//CHECKING PARENT AFFINITY
	/*pid_t parent = getpid();
	char parent_pid[10];
	sprintf(parent_pid, "%d", parent);
	char check_parent[20] = "taskset -p ";
	strcat(check_parent, parent_pid);
	system(check_parent);*/ 

//WARM UP==========================================================
	warmup();
	warmup();
	warmup();

//OBTAIN AVG OVERHEAD COSTS========================================
	double gettimeCost = timeCost();
	double rCost = readAverage();
	double wCost = writeAverage();
	double yieldCost = yieldAverage();

//SETTING UP PIPES=================================================
	int parent2child[2], child2parent[2];
	
	if(pipe(parent2child) < 0){
		perror("Pipe Fail");
	}
	if(pipe(child2parent) < 0){
		perror("Pipe Fail");
	}

//CONTEXT SWITCH MEASURE===================================	
	char c = '!'; 	// 1-byte (4-bit) message
	char r;			// for the read function to store a byte
		
	pid_t child = fork();
		
	if(child < 0){ // failure
		perror("Forking Fail");
		exit(0);
	}

	else if (child > 0 ){ // parent
		
	 //CHECKING CHILD AFFINITY (CORE)
/*		char child_pid[10];
		sprintf(child_pid, "%d", child);

		char check_child[20] = "taskset -p ";
		strcat(check_child, child_pid);
		system(check_child); 
*/
		
		close(parent2child[0]);
		close(child2parent[1]);

		// send message to child 
		write(parent2child[1], &c, 1 ); 
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		// wait for reply
		read(child2parent[0], &r, 1);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		wait(NULL);
		
	}

	else if(child == 0 ){ // child
		read(parent2child[0], &r, 1);	
		write(child2parent[1], &r, 1);
		sched_yield();			//force child off CPU
		
		close(parent2child[1]);	//close pipes at the end to reduce overhead
		close(child2parent[0]);
		exit(0);
	}
					
	result=timespecDiff(&stop,&start);
	printf("Context Switch Cost: %.1f ns\n", result/2.0 - wCost - gettimeCost- rCost*2 - yieldCost);
	return 0;
} 
