#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}


int c = 0;
void *fnC()
{
    int i;
    for(i=0;i<1000000;i++)
    {   
        c++; 
    }   
}


pthread_mutex_t count_mutex;

void *fnCLock()
{
    int i;
    for(i=0;i<1000000;i++)
    { 
	pthread_mutex_lock(&count_mutex);  
        c++;
	pthread_mutex_unlock(&count_mutex);
    }   
}


int main()
{


struct timespec start;
struct timespec stop;
unsigned long long result; //64 bit integer


    int rt1, rt2, rt3, rt4;
    pthread_t t1, t2, t3 ,t4; 
	c=0;

clock_gettime(CLOCK_MONOTONIC, &start);
    /* Create 4 threads */
    if( (rt1=pthread_create( &t1, NULL, &fnC, NULL)) )
        printf("Thread creation failed: %d\n", rt1);
 if( (rt2=pthread_create( &t2, NULL, &fnC, NULL)) )
        printf("Thread creation failed: %d\n", rt2);
 if( (rt3=pthread_create( &t3, NULL, &fnC, NULL)) )
        printf("Thread creation failed: %d\n", rt3);
if( (rt4=pthread_create( &t4, NULL, &fnC, NULL)) )
        printf("Thread creation failed: %d\n", rt4);

    /* Wait for all threads to finish */
    pthread_join( t1, NULL);
    pthread_join( t2, NULL);
    pthread_join( t3, NULL);
    pthread_join( t4, NULL);
clock_gettime(CLOCK_MONOTONIC, &stop);

    printf("Threaded Run (No Sync) %d", c);
    printf ("\n");
result=timespecDiff(&stop,&start);

printf("No Sync time(ms): %llu\n",result/1000000);
    c=0;
printf ("\n");

 clock_gettime(CLOCK_MONOTONIC, &start);
   
    /* Create 4 threads */
    if( (rt1=pthread_create( &t1, NULL, &fnCLock, NULL)) )
        printf("Thread creation failed: %d\n", rt1);
 if( (rt2=pthread_create( &t2, NULL, &fnCLock, NULL)) )
        printf("Thread creation failed: %d\n", rt2);
 if( (rt3=pthread_create( &t3, NULL, &fnCLock, NULL)) )
        printf("Thread creation failed: %d\n", rt3);
if( (rt4=pthread_create( &t4, NULL, &fnCLock, NULL)) )
        printf("Thread creation failed: %d\n", rt4);

    /* Wait for all threads to finish */
    pthread_join( t1, NULL);
    pthread_join( t2, NULL);
    pthread_join( t3, NULL);
    pthread_join( t4, NULL);

clock_gettime(CLOCK_MONOTONIC, &stop);

    printf("Threaded Run (Mutex) %d", c);
    printf ("\n");
result=timespecDiff(&stop,&start);

printf("Mutex time(ms): %llu\n",result/1000000);
    printf ("\n");
printf ("\n");

clock_gettime(CLOCK_MONOTONIC, &start);
	c=0;
    int i;
    for(i=0;i<400000000;i++)
    {   
        c++; 
    }     
	clock_gettime(CLOCK_MONOTONIC, &stop);
result=timespecDiff(&stop,&start);

printf("1 thread: %llu\n",result/1000000);

	return 0;

}