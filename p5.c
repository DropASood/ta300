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
#include <pthread.h>

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}
//part 5
int main(){

//INITIALIZATION
	struct timespec start; 			//variable that stores start time
	struct timespec stop;			//variable that stores end time
	signed long long int result; 		//64 bit integer
		
	pid_t parent = getpid();
	char parent_pid[10];
	sprintf(parent_pid, "%d", parent);


	//SETTING PARENT AFFINITY (CORE)
	char set_core[30] = "taskset -cp 0 ";
	strcat(set_core, parent_pid);
	system(set_core);

	
/*	//CHECKING PARENT AFFINITY (CORE)
	char check_parent[20] = "taskset -p ";
	strcat(check_parent, parent_pid);
	system(check_parent);

*/
	

	return 0;
} 
