// CMPT-300 Assignment 2
// Part I: Measuring Context Switch Costs
// Archit Sood 301188293
// Tony Dinh 301142069

#include <stdio.h>
#include <stdint.h>
#include <time.h>

/*

CLOCK_REALTIME
    System-wide realtime clock. Setting this clock requires appropriate privileges. 

CLOCK_MONOTONIC
    Clock that cannot be set and represents monotonic time since some unspecified starting point. 

CLOCK_PROCESS_CPUTIME_ID
    High-resolution per-process timer from the CPU. 

CLOCK_THREAD_CPUTIME_ID
    Thread-specific CPU-time clock. 

*/


unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

void barefunction(){
	return;
}




int main(){

struct timespec start;
struct timespec stop;
unsigned long long result; //64 bit integer

//CLOCK_REALTIME

	clock_gettime(CLOCK_REALTIME, &start); 
	sleep(1);
	clock_gettime(CLOCK_REALTIME, &stop);

	result=timespecDiff(&stop,&start);

	printf("CLOCK_REALTIME Measured: %llu\n",result);

//CLOCK_MONOTONIC
	clock_gettime(CLOCK_MONOTONIC, &start);
	sleep(1);
	clock_gettime(CLOCK_MONOTONIC, &stop);

	result=timespecDiff(&stop,&start);

	printf("CLOCK_MONOTONIC Measured: %llu\n",result);

//CLOCK_PROCESS_CPUTIME_ID
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	sleep(1);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	result=timespecDiff(&stop,&start);

	printf("CLOCK_PROCESS_CPUTIME_ID Measured: %llu\n",result);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
	sleep(1);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);

	result=timespecDiff(&stop,&start);

	printf("CLOCK_THREAD_CPUTIME_ID Measured: %llu\n",result);

	return 0;
}

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
	It allows to measure not the elapsed system time, but the time taken 
	by the process itself. However, due to some hardware having seperate timers
	on each CPU, the result may be invalid if the process is moved between CPUs. [2]

	We would use CLOCK_PROCESS_CPU_ID when we wish to measure the total time
	taken by a process.

CLOCK_THREAD_CPUTIME_ID

References: 

[1] http://nadeausoftware.com/articles/2012/04/c_c_tip_how_measure_elapsed_real_time_benchmarking
[2] https://upvoid.com/devblog/2014/05/linux-timers/
*/ 