//+
//	ELEC 377,  Lab 3
//
//  producer.c
//  The producer reads in text from the input test file.
// 	It then stores the text in the shared queue.
//  
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1


int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
	int c,stored;
    
	/*	 Shared memory init 	*/
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(sharedPtr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}
	
    // put your code here..
	getMutex(&sharedPtr -> lock);
	sharedPtr -> numProducers++;
	releaseMutex(&sharedPtr -> lock);
	while((c = getchar()) != EOF)
	{
		stored = FALSE;
		while(stored == FALSE)
		{
			getMutex(&sharedPtr -> lock);

			// int counter;
			// int i;
			// for (i = 0; i < BUFFSIZE; i++) //checking the amount of elements in the buffer array
			// {
			// 	if (sharedPtr -> buffer[i] != NULL || sharedPtr -> buffer[i] != '\0')
			// 	{
			// 		counter++;
			// 	}
			// }
			// if (counter != BUFFSIZE) //checking if the buffer array still has room left in it (i.e. it is not full)
			// {
			// 	stored = TRUE;
			// }

			if(sharedPtr -> count < BUFFSIZE)
			{
				sharedPtr -> buffer[sharedPtr -> in] = c;
				sharedPtr -> in = (sharedPtr -> in + 1) % BUFFSIZE;
				sharedPtr -> count++;
				stored = TRUE;
			}
			
			releaseMutex(&sharedPtr -> lock);
		}
	}
	getMutex(&sharedPtr -> lock);
	sharedPtr -> numProducers--;
	releaseMutex(&sharedPtr -> lock);
	return 0;
}
