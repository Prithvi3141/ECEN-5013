#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <mqueue.h>
#include <errno.h>

#include "IPC_msg.h"


/*#define QUEUE_NAME "/example_mq"
#define QUEUE_PERMISSIONS 0666
#define MAX_STRING_SIZE 1024*/
/*
typedef struct
{
	char string[BUFFER_SIZE];
    	int string_length;
}string_info;

typedef struct 
{
	string_info info;
    	bool switch_status;
}message;
*/
void  child_process(void)
{

        char buffer[MAX_STRING_SIZE] = {0};

        mqd_t msg_q;
        struct msg_buffer message;
        struct mq_attr *attr1;

	bzero(buffer, sizeof(struct msg_buffer));
	bzero(&message, sizeof(&message));


	printf(" \n");
	
	msg_q = mq_open(PATH,O_RDWR | O_CREAT, 0666, NULL);
	if(msg_q == -1) 
	{	
		printf("Error in opening message_queue\n");
		exit(1);
	}

	attr1 = malloc(sizeof(struct mq_attr));
	mq_getattr(msg_q,attr1);

	if( mq_receive(msg_q,(char *)&message,attr1->mq_msgsize,NULL) == -1)
	{
		printf("Error in receiving message on message_queue with erro:%s\n",strerror(errno));		
	}
	else
	{

        	printf("\nMaster Process:\tThe sent message is %s", (message.string_message).string);
                fflush(stdout);
                bzero(&message, sizeof(message));
                message.led_state = true;
	        if( mq_send(msg_q,(char *)&message,sizeof(message),1)== -1)
		{
                	printf("Error in sending message on message_queue with erro:%s\n",strerror(errno));
          	}
	}
	

	mq_close(msg_q);
	mq_unlink(PATH);

	printf("End of the child process \n");
	fflush(stdout);
	
}

void  parent_process(void)
{
    	printf("Start of the parent process \n"); 
	
	mqd_t mq1;
	
	struct msg_buffer message;
	char buffer[MAX_STRING_SIZE] ={0};

	bzero(&message, sizeof(struct msg_buffer));
        bzero(buffer, sizeof(buffer));

        strcpy(buffer, "Should I Switch on the LED on Beaglebone?");

	strncpy(message.string_message.string , buffer, strlen(buffer));
	message.string_message.string_length = strlen(buffer);
//	message.led_state = true;	
	
	mq1 = mq_open(PATH,O_RDWR | O_CREAT, 0666, NULL);
	if(mq1 == -1) 
	{	
		printf("Error in opening message_queue\n");
		exit(1);
	}

	if( mq_send(mq1,(char *)&message,sizeof(message),1)== -1){
		printf("Error in sending message on message_queue\n");
	}


//	msg_buffer receive_message;
	struct mq_attr *attr1;
	attr1 = malloc(sizeof(struct mq_attr));
	mq_getattr(mq1,attr1);

	bzero(&message, sizeof(message));	


	if( mq_receive(mq1,(char *)&message,attr1->mq_msgsize,NULL) == -1)
	{
		
		printf("Error in receiving message on message_queue with erro:%s\n",strerror(errno));
			
	}
	else
	{
        	if(message.led_state)
                	printf("\nSlave process:\tThe LED is switched on.\n");
                else
                	printf("\nSlave Process:\tThe LED is switched off.\n");
                fflush(stdout);
	}

	mq_close(mq1);
	mq_unlink(PATH);
	free(attr1);
	printf("End of the parent process \n");
	fflush(stdout);
}

int  main(void)
{
	pid_t  pid;

	pid = fork();
	if (pid != 0) 
	  child_process();
	else 
	  parent_process();
	return 0;
}



