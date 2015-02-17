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

int main(){

//INITIALIZATION===================================================
	struct timespec start; 			//variable that stores start time
	struct timespec stop;				//variable that stores end time
	unsigned long long result; 	//64 bit integer

//WARM UP================================
/*	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	barefunction();
	*/

//OVERHEAD OF START & STOP===============
/*	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("Cached Start & Stop Cost: %llu ns\n\n",result);
*/

	//===================================

		clock_gettime(CLOCK_MONOTONIC, &start);
		barefunction();
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		printf("Cached barefunction Cost: %llu ns\n\n",result);



	

	return 0;
}