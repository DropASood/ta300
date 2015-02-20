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
unsigned long long result; 		//64 bit integer

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

void barefunction(){}

double loopCost(int iterations){

	for(int j = 0; j < 20; j++){
		clock_gettime(CLOCK_MONOTONIC, &start);  
		for(int i = 0 ; i < iterations; i++);
		clock_gettime(CLOCK_MONOTONIC, &stop);	
		result = result + timespecDiff(&stop,&start);
	}
	return result/20.0;

}

double timeCost(int iterations){
	double sum = 0;

	for(int i = 0 ; i < iterations ; i++){
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		result=timespecDiff(&stop,&start);
		sum = sum + result;
	}

	double average = sum/(float)iterations;
	return average;
}
int main(){

//INITIALIZATION===================================================
	signed long long int timed[21]={};

//WARM UP================================
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	

//OVERHEAD OF START & STOP===============
/*	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result=timespecDiff(&stop,&start);
	printf("Cached Start & Stop Cost: %llu ns\n\n",result);
*/

	//===================================
		double tcost = timeCost(1000);
		double average = 0;

		
		for(int i = 0; i < 21 ; i++){
			
			clock_gettime(CLOCK_MONOTONIC, &start);
			barefunction();
			clock_gettime(CLOCK_MONOTONIC, &stop);

			result=timespecDiff(&stop,&start);
			timed[i] = result - tcost;
			if(i != 0) average = average + timed[i]; //ignore first entry;
			printf("%.1f ns\n", (float)timed[i]);
		
		}
		average = average/20.0;




		printf("Cached barefunction Cost: %f ns\n\n",average);



	

	return 0;
}