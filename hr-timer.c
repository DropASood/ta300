// CMPT-300 Assignment 2
// Part I: Measuring Context Switch Costs
// Archit Sood 301188293
// Tony Dinh 301142069

/*=============
   QUESTION 1
===============*/

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

int main(){

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

	result=timespecDiff(&stop,&start);					//compute 

	printf("CLOCK_PROCESS_CPUTIME_ID Measured: %llu\n",result); //display result

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);		//record thread start time
	sleep(1);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);		//record thread end time

	result=timespecDiff(&stop,&start);					//compute difference

	printf("CLOCK_THREAD_CPUTIME_ID Measured: %llu\n",result);	//display result


return 0;
}