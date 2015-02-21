// // CMPT-300 Assignment 2
// // Part I: Measuring Context Switch Costs
// // Archit Sood 301188293
// // Tony Dinh 301142069

/*=============
   PROBLEM 5
===============*/

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int shared_num = 0;	//shared integer between two threads
int switch_count = 0;	//number of thread switches
pthread_mutex_t mutex1, mutex2 = PTHREAD_MUTEX_INITIALIZER; // two locks for the critical sections
pthread_cond_t cond1, cond2 = PTHREAD_COND_INITIALIZER;

struct timespec start; 			//variable that stores start time
struct timespec stop;			//variable that stores end time
unsigned long long int result; 	//64 bit integer

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
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

// Function for thread 1 to change shared number to 0
void *change_Num0(){
    while (switch_count < 100000) { //loop while there has been less than 100000 thread switches
    	
	    pthread_mutex_lock(&mutex1);  //lock critical section 
	    	
	   	while(shared_num == 0) {	//if num is already 0, wait for thread 2 to do work
	   		pthread_cond_wait(&cond1, &mutex1); 
		}
		
		shared_num = 0;						//change num to 0
		switch_count++; 
		pthread_cond_signal(&cond2); 	//inform thread 2 that num has been changed
	    pthread_mutex_unlock(&mutex1); 	//unlock critical section
    }

    return 0;
}

// Function for thread 2 to changed shared number to 1
void *change_Num1 () {
    while (switch_count < 100000) { //loop while there has been less than 100000 thread switches

        pthread_mutex_lock(&mutex2);	//lock critical section 
        
        while(shared_num == 1) {	//if num is already 1, wait for thread 1 to do work
            pthread_cond_wait(&cond2, &mutex2);
        }

	    shared_num = 1;						//change num to 1
	    switch_count++;  
	    pthread_cond_signal(&cond1); 	//inform thread 2 that num has been changed
	    pthread_mutex_unlock(&mutex2); 	//unlock critical section
    } 

    return 0;
}

int main () {

//SETTING AFFINITY========================================================
	cpu_set_t set;                   				// Define a set of CPUs
    CPU_ZERO (&set);               					// Clear the set so there is no CPUs
    CPU_SET (1, &set);              				// Add a core to the set
    sched_setaffinity(0, sizeof(cpu_set_t), &set); 	// Set affinity of this process to the defined mask

//CREATING THREADS & MEASURING THREAD SWITCH==============================    
    pthread_t thread1, thread2;	// Thread ID for our 2 threads
    
    
    //Start Timer
    clock_gettime (CLOCK_MONOTONIC, &start);
    
    // Create 2 threads
    pthread_create (&thread1, NULL, &change_Num0, NULL);
    pthread_create (&thread2, NULL, &change_Num1, NULL);

    // Wait for both threads to finish
    pthread_join (thread1,NULL);
    pthread_join(thread2, NULL);

    //End Timer
    clock_gettime (CLOCK_MONOTONIC, &stop);

	result=timespecDiff(&stop,&start); // Difference is the lifespan of the two threads


	double average = (result/(double)switch_count) - timeCost();
    printf("Average cost of thread switch: %.1f ns\n", average);  

    return 0;

}


