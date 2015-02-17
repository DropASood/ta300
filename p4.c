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
//part 4
int main(){

//INITIALIZATION
	struct timespec start; 			//variable that stores start time
	struct timespec stop;			//variable that stores end time
	signed long long int result; 		//64 bit integer
		
	pid_t parent = getpid();
	//printf("parent PID = %i\n\n",parent);
	char parent_pid[10];
	sprintf(parent_pid, "%d", parent);

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
	pid_t child = fork();

	

	if(child < 0){ //child
		perror("Forking Fail");
		exit(0);
	}

	else if (child > 0 ){ //parent
		
/*		//CHECKING CHILD AFFINITY (CORE)
		char child_pid[10];
		sprintf(child_pid, "%d", child);

		char check_child[20] = "taskset -p ";
		strcat(check_child, child_pid);
		system(check_child);*/

		//SEND 1 BYTE TO CHILD
		char c = '!'; // 1-byte (4-bit) message
		char r;
		close(parent2child[0]);
		close(child2parent[1]);

		// send message to child & wait for reply
		clock_gettime(CLOCK_MONOTONIC, &start);
		if(write(parent2child[1], &c, 1 ) != 1){
			printf("Parent failed to send message\n");
		}

		if(read(child2parent[0], &r, 1) != 1){
			printf("Parent failed tp read reply\n");
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		wait(NULL);
			
	}

	else if(child == 0 ){
		close(parent2child[1]);
		close(child2parent[0]);
		char r;
/*		FILE* read_f = fdopen(parent2child[0], "r");
		FILE* write_f = fdopen(child2parent[1], "w");*/

		if(read(parent2child[0], &r, 1) != 1){
			printf("Child failed to read message\n");
		}

		if(write(child2parent[1], &r, 1) !=1){
			printf("Child failed to send reply\n");
		}
		exit(0);
	}

	result=timespecDiff(&stop,&start);
	printf("%lli ns\n", result );
	return 0;
} 
