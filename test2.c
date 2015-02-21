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


