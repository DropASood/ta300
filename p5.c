// // CMPT-300 Assignment 2
// // Part I: Measuring Context Switch Costs
// // Archit Sood 301188293
// // Tony Dinh 301142069
// #include <stdio.h>
// #include <stdint.h>
// #include <time.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <errno.h>
// #include <sys/syscall.h> 
// #include <pthread.h>

// int shared_int = 0; //the shared integer of both threads
// pthread_t tid[2];	//storage for thread IDs			
// pthread_mutex_t mutex;


// unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
// {
//   return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
//            ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
// }


// void *dowork(){
	
// 	pthread_t id = pthread_self();

// 	pthread_mutex_lock(&mutex);
	
// 	if(pthread_equal(id, tid[0]) && shared_int == 1){
// 		shared_int = 0;
// 	}

// 	else{
// 		if(shared_int == 0){
// 			shared_int = 1;
// 		}
// 	}
// 	pthread_mutex_unlock(&mutex);

// }

// double gettimeAverage(){
// 	signed long long int sum = 0;

// 	for(int i = 0 ; i < 100 ; i++){
// 		clock_gettime(CLOCK_MONOTONIC, &start);
// 		clock_gettime(CLOCK_MONOTONIC, &stop);
// 		result=timespecDiff(&stop,&start);
// 		sum = sum + result;
// 	}

// 	double average = sum/100.0;
// 	return average;
// }

// int main(){

// //part 5
// //INITIALIZATION==========================================================
// 	struct timespec start; 			//variable that stores start time
// 	struct timespec stop;			//variable that stores end time
// 	signed long long int result; 	//64 bit integer

// 	pid_t parent = getpid();
// 	char parent_pid[10];
// 	sprintf(parent_pid, "%d", parent);


// 	//SETTING PARENT AFFINITY (CORE) -- Forcing it to run on 1 core
// 	char set_core[30] = "taskset -cp 0 ";
// 	strcat(set_core, parent_pid);
// 	system(set_core);

// 	/*//CHECKING PARENT AFFINITY (CORE)
// 	char check_parent[20] = "taskset -p ";
// 	strcat(check_parent, parent_pid);
// 	system(check_parent);
// */


// //CREATING THREADS========================================================
// 	if(pthread_create(&tid[0], NULL, &dowork,NULL)!=0){
// 		perror("Thread Create Fail");
// 	}
// 	if(pthread_create(&tid[1], NULL, &dowork,NULL)!=0){
// 		perror("Thread Create Fail");
// 	}

// 	pthread_join(tid[0], NULL);
// 	pthread_join(tid[1], NULL);
// 	pthread_t id = pthread_self();

// 	clock_gettime(CLOCK_MONOTONIC, &start);


// 	int i=0;
// 	while (i<1000){
// 		if(pthread_equal(id, tid[0]) && shared_int == 1){
// 			shared_int = 0;
// 		}

// 		else{
// 			if(shared_int == 0){
// 				shared_int = 1;
// 			}
// 		}
// 		i++;
// 	}
	
// 	clock_gettime(CLOCK_MONOTONIC, &stop);
// 	result=timespecDiff(&stop,&start);
// 	printf("%llu\n", result);

// 	return 0;
// } 



#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>



int num = 0;
int count = 0;
pthread_mutex_t mutex1, mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1, cond2 = PTHREAD_COND_INITIALIZER;


void *changeNumValue1 () {

    while (1) { 
    pthread_mutex_lock(&mutex1);   

    while(num == 0) {

    pthread_cond_wait(&cond1, &mutex1);

}

num = 0;

pthread_cond_signal(&cond2); 

    pthread_mutex_unlock(&mutex1);

    count++; 

    }

}


void *changeNumValue2 () {

    while (1) { 

        pthread_mutex_lock(&mutex2);
        while(num == 1) {
            pthread_cond_wait(&cond2, &mutex2);
        }

        num = 1;
        pthread_cond_signal(&cond1); 
        pthread_mutex_unlock(&mutex2); 

        count++;  

    } 

}

int main () {

    cpu_set_t mask;                                     // Define cpu_set bit mask
    CPU_ZERO (&mask);                                   // Initialize all to 0, i.e. no CPUs selected
    CPU_SET (1, &mask);                                 // Set the bit that represents core 1
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);     // Set affinity of this process to the defined mask



    struct timespec t1, t2;

    pthread_t thread1, thread2;



    pthread_create (&thread1, NULL,(void *) &changeNumValue1, NULL);

    pthread_create (&thread2, NULL,(void *) &changeNumValue2, NULL);

    pthread_detach (thread1);

    pthread_detach (thread2);



    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &t1);

    sleep(10);

    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &t2);

  

    // average time per thread switching 

    printf("Average time for thread switching:\t%.2f ns\n", 

        (((double) t2.tv_nsec - t1.tv_nsec)/count));    

  

    return 0;

}


