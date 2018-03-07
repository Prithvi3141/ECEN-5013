#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <mqueue.h>
#include <errno.h>

#include "IPC_msg.h"

sem_t semaphore_share;


int  main(void)
{

	if( sem_init(&semaphore_share,1,1) < 0){
		
	  perror("semaphore initilization");
	  exit(0);
	}
	pid_t  pid;

	pid = fork();
	if (pid != 0)
	{


        char buffer[MAX_STRING_SIZE] = {0};
        struct msg_buffer message;        

	bzero(buffer, sizeof(struct msg_buffer));
	bzero(&message, sizeof(&message));

	/* Name of the shared memory object */
	const char *name_shared_memory = "SHARED_MEMORY_EXAMPLE";
	
	/* Shared memory file descriptor */
	int shm_fd;
	
	/* Pointer to the shared memory object */
	void *shared_memory_ptr;
	
	/* Creating shared memory object */
	shm_fd = shm_open(name_shared_memory, O_CREAT | O_RDWR, 0666);
	
	/* Memory mapping the shared memory object */
	shared_memory_ptr = mmap(0, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


	printf(" \n");

	sem_wait(&semaphore_share);
	/* Receiving message on shared memory */
	memcpy((char *)&message, shared_memory_ptr, sizeof(message));
	msync(shared_memory_ptr,sizeof(message),MS_SYNC);
	sem_post(&semaphore_share);

	printf("Master Process:\tThe sent message is %s", (message.string_message).string);
        fflush(stdout);

	bzero(&message, sizeof(message));
        message.led_state = true;

	sem_wait(&semaphore_share);
	/* Sending message on shared memory */
	memcpy(shared_memory_ptr, (char *)&message , sizeof(message));
	msync(shared_memory_ptr,sizeof(message),MS_SYNC);
	sem_post(&semaphore_share); 
	}
	else
	{
	struct msg_buffer message;
	char buffer[MAX_STRING_SIZE] ={0};

	bzero(&message, sizeof(struct msg_buffer));
        bzero(buffer, sizeof(buffer));

	/* Name of the shared memory object */
	const char *name_shared_memory = "SHARED_MEMORY_EXAMPLE";
	
	/* Shared memory file descriptor */
	int shm_fd;
	
	/* Pointer to the shared memory object */
	void *shared_memory_ptr;
	
	/* Creating shared memory object */
	shm_fd = shm_open(name_shared_memory, O_CREAT | O_RDWR, 0666);
	
	/* Configuring size of shared memory object */
	ftruncate(shm_fd, sizeof(message));
	
	/* Memory mapping the shared memory object */
	shared_memory_ptr = mmap(0, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        strcpy(buffer, "Should I Switch on the LED on Beaglebone?");
	printf("Slave Process:\t%s", buffer);
	strncpy(message.string_message.string , buffer, strlen(buffer));
	message.string_message.string_length = strlen(buffer);
//	message.led_state = true;	

	sem_wait(&semaphore_share);
	/* Sending message on shared memory */
	memcpy(shared_memory_ptr, (char *)&message, sizeof(message));
	msync(shared_memory_ptr,sizeof(message),MS_SYNC);
	sem_post(&semaphore_share);
	sleep(0.5);

	bzero(&message, sizeof(message));	
	sem_wait(&semaphore_share);
	/* Receiving message on shared memory */
	memcpy((char *)&message,shared_memory_ptr, sizeof(message));
	msync(shared_memory_ptr,sizeof(message),MS_SYNC);
	sem_post(&semaphore_share);

	if(message.led_state)
        	printf("\nSlave process:\tThe LED is switched on.\n");
        else
        	printf("\nSlave Process:\tThe LED is switched off.\n");
        fflush(stdout);
	
	}
	return 0;
}



