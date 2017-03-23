/* Dinesh Ramamoorthy */
/* U51857354 */

#define _REENTRANT
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
/*Tested on linux 1/23/11 */
/* compile with gcc -pthread -o filename filename.c */

struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };
/* the code is timed here with getrusage */

struct rusage mytiming;
struct timeval mytimeval;
struct shared_dat  *counter;
int temp;

/****************************************************************
 * * This function increases the value of shared variable "counter"
 *   by one 2000000 times
 *   ****************************************************************/
/* pthread mutex is initiated here */
/* reffered from the linux pthread webpage given in the project instructions */

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void * thread1(void *arg)
{
	int line = 0;
	int ind_count = 0;
	while (line < 2000000)
	{
 	    line++;
	    
	    while(pthread_mutex_trylock(&mutex1)==EBUSY);
	    /* Critical Section */
            /* The condition to be modified in thread 1 is added here */
	    if((counter->value%100)==0)
	    {
		if(line+100<2000000)
		{
			ind_count++;
			counter->value = counter->value+100;
			line+=100;
		}
	    }

	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;

	
	    pthread_mutex_unlock(&mutex1);

         }
	 printf("from process1 counter  =  %d\n", counter->value); 
	 printf("from thread1 %d 100 counts were performed.\n", ind_count);
 	 return(NULL);
}


/****************************************************************
 * This function increases the value of shared variable "counter"
 * by one 2000000 times
 * ****************************************************************/
void * thread2(void *arg)
{
	int line = 0;	
	while (line < 2000000)
	   {
            line++;
		
	    while(pthread_mutex_trylock(&mutex1)==EBUSY);
/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;

	    pthread_mutex_unlock(&mutex1);
	    
           }
	   printf("from process2 counter = %d\n", counter->value); 
return(NULL);
}


/****************************************************************
 * *                  Main Body                                    *
 * ****************************************************************/
main()
{
/* the code is timed here with getsurage */

	getrusage(RUSAGE_SELF, &mytiming);
	mytimeval = mytiming.ru_stime;	        

	int             r=0;
	int 		i;
	int 		shmid;	  /* shared memory ID */
        pthread_t	tid1[1];     /* process id for thread 1 */
        pthread_t	tid2[1];     /* process id for thread 2 */
        pthread_attr_t	attr[1];     /* attribute pointer array */

         counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));

	/* initialize shared memory to 0 */
	counter->value = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
    
	fflush(stdout);
 /* Required to schedule thread independently.
 *  Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 

	/* end to schedule thread independently */

/* Create the threads */
        pthread_create(&tid1[0], &attr[0], thread1, NULL);
        pthread_create(&tid2[0], &attr[0], thread2, NULL);

/* Wait for the threads to finish */
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
  
	        printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");

getrusage(RUSAGE_SELF, &mytiming);
mytimeval = mytiming.ru_stime;

printf("Time used is sec: %d, usec %d\n",mytiming.ru_utime.tv_sec,
mytiming.ru_utime.tv_usec);
printf("System Time used is sec: %d, usec %d\n",mytiming.ru_stime.tv_sec,
mytiming.ru_stime.tv_usec);

		exit(0);
		
}

