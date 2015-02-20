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

struct timespec start; 			//variable that stores start time
struct timespec stop;			//variable that stores end time
signed long long int result; 	//64 bit integer

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

void warmup(){
	int fd[1];
	char c = '!';
	char r; 
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	write(fd[0], &c, 1 );
	read(fd[0], &r, 1 );
	close(fd[0]);
}

double gettimeAverage(){
	signed long long int sum = 0;

	for(int i = 0 ; i < 20 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}

	double average = sum/20.0;
	return average;
}

double readAverage(){
	char r;
	int fd[1]; char c = '!';
	signed long long int sum = 0;
	for(int i = 0 ; i < 20 ; i++){
		write(fd[0], &c, 1 );
		clock_gettime(CLOCK_MONOTONIC, &start);
		read(fd[0], &r, 1 );
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}
	
	return sum/20.0;

}

double writeAverage(){
	int fd;
	char c = '\0';
	signed long long int sum = 0;
	for(int i = 0 ; i < 20 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		write(fd, &c, 1 );
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}
	
	return sum/20.0;

}


//part 4
int main(){

//INITIALIZATION===========================================
		
	pid_t parent = getpid();
	char parent_pid[10];
	sprintf(parent_pid, "%d", parent);
	//WARMUP FUNCTIONS
	warmup();
	warmup();
	warmup();

	//GET AVERAGE OVERHEAD COSTS

	double gettimeCost = gettimeAverage();
	double rCost = readAverage();
	double wCost = writeAverage();

	printf("gettimeCost: %f\nrCost: %f\nwCost: %f\n",gettimeCost, rCost,wCost );



	//SETTING UP PIPES
	int parent2child[2], child2parent[2];
	
	if(pipe(parent2child) < 0){
		perror("Pipe Fail");
	}
	if(pipe(child2parent) < 0){
		perror("Pipe Fail");
	}

	//SETTING PARENT AFFINITY (CORE)
	char set_core[30] = "taskset -cp 0 ";
	strcat(set_core, parent_pid);
	system(set_core);

	
/*	//CHECKING PARENT AFFINITY (CORE)
	char check_parent[20] = "taskset -p ";
	strcat(check_parent, parent_pid);
	system(check_parent);

*/
//CONTEXT SWITCH MEASURE===================================	
	char c = '!'; // 1-byte (4-bit) message
	char r;
	signed long long int sum = 0;
	
	
				
		pid_t child = fork();
		
		if(child < 0){ //child
			perror("Forking Fail");
			exit(0);
		}

		else if (child > 0 ){ //parent.. master updated
					
			/*		//CHECKING CHILD AFFINITY (CORE)
					char child_pid[10];
					sprintf(child_pid, "%d", child);

					char check_child[20] = "taskset -p ";
					strcat(check_child, child_pid);
					system(check_child);*/

					//SEND 1 BYTE TO CHILD
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

		else if(child == 0 ){
					close(parent2child[1]);
					close(child2parent[0]);
					read(parent2child[0], &r, 1);	
					write(child2parent[1], &r, 1);			
					exit(0);
				}
					
	
	
	result=timespecDiff(&stop,&start);
	

	printf("Time before subtraction: %f\n", result/2.0);
	printf("%f ns\n", result/2.0 - wCost - gettimeCost- rCost*2);
	return 0;
} 
