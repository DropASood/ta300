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

int shared_int = 0; //the shared integer of both threads
pthread_t tid[2];	//storage for thread IDs			
pthread_mutex_t mutex; 

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

void *dowork(){
	pthread_t id = pthread_self();

	pthread_mutex_lock(&mutex);
	
	if(pthread_equal(id, tid[0]) && shared_int == 1){
		shared_int = 0;
	}

	else{
		if(shared_int == 0){
			shared_int = 1;
		}
	}
	pthread_mutex_unlock(&mutex);

	return;
}

double gettimeAverage(){
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

int main(){

//part 5
//INITIALIZATION==========================================================
	struct timespec start; 			//variable that stores start time
	struct timespec stop;			//variable that stores end time
	signed long long int result; 	//64 bit integer
		
	pid_t parent = getpid();
	char parent_pid[10];
	sprintf(parent_pid, "%d", parent);


	//SETTING PARENT AFFINITY (CORE) -- Forcing it to run on 1 core
	char set_core[30] = "taskset -cp 0 ";
	strcat(set_core, parent_pid);
	system(set_core);

	/*//CHECKING PARENT AFFINITY (CORE)
	char check_parent[20] = "taskset -p ";
	strcat(check_parent, parent_pid);
	system(check_parent);
*/


//CREATING THREADS========================================================
	if(pthread_create(&tid[0], NULL, &dowork,NULL)!=0){
		perror("Thread Create Fail");
	}
	if(pthread_create(&tid[1], NULL, &dowork,NULL)!=0){
		perror("Thread Create Fail");
	}

	pthread_join(&tid[0], NULL);
	pthread_join(&tid[1], NULL);

	return 0;
} 
