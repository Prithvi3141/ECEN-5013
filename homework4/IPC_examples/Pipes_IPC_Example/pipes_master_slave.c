#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

#include "IPC_msg.h"

int main()
{
	int master_to_slave_pipefd[2], slave_to_master_pipefd[2], childPID, retval;
	char buffer[MAX_STRING_SIZE];

	retval = pipe(master_to_slave_pipefd);
	if(retval)
	{
		perror("Error creating a master to slave half duplex pipe");
		exit(1);
	}
	retval = pipe(slave_to_master_pipefd);
        if(retval)
        {
                perror("Error creating a slave to master half duplex pipe");
		close(master_to_slave_pipefd[0]);
		close(master_to_slave_pipefd[1]);
                exit(1);
        }

	childPID = fork();

	if(childPID<0)
	{
		perror("Error creating a child process");
		close(master_to_slave_pipefd[0]);
                close(master_to_slave_pipefd[1]);
                close(slave_to_master_pipefd[0]);
                close(slave_to_master_pipefd[1]);
		exit(1);
	}
	//Code block executed by slave 
	else if(childPID==0)
	{
		close(master_to_slave_pipefd[1]);
		close(slave_to_master_pipefd[0]);
		
                msg_buffer msg;
                char buffer[MAX_STRING_SIZE];

                bzero(&msg, sizeof(msg));
                bzero(buffer, sizeof(buffer));

                sprintf(buffer, "Should I Switch on the LED on Beaglebone?");
                printf("\nSlave Process:\t%s", buffer);
                fflush(stdout);

                (msg.string_message).string_length = strlen(buffer);
                strcpy((msg.string_message).string, buffer);

                retval = write(slave_to_master_pipefd[1],  &msg, STRUCT_SIZE);
                if(retval > 0)
                {
                	bzero(&msg, sizeof(msg));
                        retval = read(master_to_slave_pipefd[0], &msg, STRUCT_SIZE);
                        if(retval>0)
                        {
                        	if(msg.led_state)
                                	printf("\nSlave process:\tThe LED is switched on.\n");
                                else
                                	printf("\nSlave Process:\tThe LED is switched off.\n");
                                fflush(stdout);
                        }
                }
                close(master_to_slave_pipefd[0]);
                close(slave_to_master_pipefd[1]);


		
	}
	//Code block executed by master	
	else if(childPID>0)
	{

                msg_buffer msg;

                close(master_to_slave_pipefd[0]);
                close(slave_to_master_pipefd[1]);

                retval = read(slave_to_master_pipefd[0], &msg, STRUCT_SIZE);
                if(retval > 0 )
                {
                        printf("\nMaster Process:\tThe sent message is %s", (msg.string_message).string);
                        fflush(stdout);
                        bzero(&msg, sizeof(msg));
                        msg.led_state = true;
                        retval = write(master_to_slave_pipefd[1],  &msg, STRUCT_SIZE);
                }

                close(master_to_slave_pipefd[1]);
                close(slave_to_master_pipefd[0]);

	}
}

