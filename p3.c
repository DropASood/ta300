// CMPT-300 Assignment 2
// Part I: Measuring Context Switch Costs
// Archit Sood 301188293
// Tony Dinh 301142069

/*=============
   PROBLEM 3
===============*/

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
unsigned long long result; 		//64 bit integer

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

// Measures an average overhead cost of clock_gettime(CLOCK_MONOTONIC, &start) & clock_gettime(CLOCK_MONOTONIC, &stop)
double timeCost(){
	double sum = 0;

	for(int i = 0 ; i < 20 ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}

	double average = sum/20.0;
	return average;
}

// Puts clock_gettime() into cache
void warmup(){
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
}

int main(){

//INITIALIZATION===================================================
	signed long long int timed[21]={}; // Stores measurements

//WARM UP==========================================================
	warmup();
	warmup();
	warmup();


//BENCHMARKING=====================================================
	double tcost = timeCost(); // Obtain overhead for timer and stop calls
	double average = 0;

	
	for(int i = 0; i < 21 ; i++){
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		getpid();
		clock_gettime(CLOCK_MONOTONIC, &stop);

		result=timespecDiff(&stop,&start);
		timed[i] = result - tcost;					//deduct overhead cost 
		if(i != 0) average = average + timed[i]; 	//ignore first entry
		printf("%.1f ns\n", (float)timed[i]);
	
	}
	average = average/21.0;
	printf("Cached syscall Cost: %f ns\n\n",average);
	
	return 0;
}