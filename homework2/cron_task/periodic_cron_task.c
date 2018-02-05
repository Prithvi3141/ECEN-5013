/*
# File_Name: periodic_cron_task.c
# Author:  Prithvi Teja Veeravalli <prithvi.veeravalli.colorado.edu>
# Description: This program is used to call the sort_array system call defined and call a few other system call APIs to collect a few information. Thsi program is going to be scheduled by cron to execute once in every 10 minutes like a daemon process.
# Version: 1.2

*/

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>

#define BUFFER_SIZE 256 

int main()
{
	int num_elements;
	int buffer_size = 40;
	int32_t a[10];
	pid_t pid;
	uid_t uid;
	time_t current_time;
	struct tm *current_tm;

	pid = getpid();
	uid = getuid();
	current_time = time(NULL);
	current_tm = localtime(&current_time);

	printf("Starting Cron task\n\n\n"); 
	
	printf("Process Id:%d\n", (int) pid);
        printf("User Id:%d\n", (int) uid);
	printf("Current Date and Time: %s\n", asctime(current_tm));
	

	int32_t b[10]; //= {82, 35, 42, 91, 93, 44, 132, 258, 110, 11};

        num_elements = buffer_size/sizeof(int32_t);

	printf("Given Array %d\n", (int )sizeof(int32_t));
        for(int i = 0; i<num_elements; i++)
        {
		b[i] = rand();
                printf("b[%d] = %d \t", i, b[i]);       
        }

	printf("\n");

	syscall(333, b, buffer_size, a);    //Calling sort_array system call by using syscall API
	
	printf("Sorted Array\n");
	for(int i = 0; i<num_elements; i++)
	{
		printf("a[%d] = %d \t", i, a[i]);	
	}
	printf("\n\n\nCron Task Completed\n"); 


	return 1;
}
