#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<pthread.h>
#include<time.h>
#include<sys/file.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<sys/syscall.h>

typedef struct info{

char *fileName;

}info;


void timer_handler(int signum);
void *file_parsing_thread(void *init_info);
void *cpu_utilization_logger_thread(void *init_info);

void logging_cpu_utilization(int logFile);
void print_to_log_file(int logFile, char *buffer, ssize_t readLength);
void print_timestamp(int logFile);



int log_cpu_utilization;
int thread_cancel[2];
int threads_closed_count = 0;
int logFile[3];
pthread_t threads[2];

pthread_mutex_t mutex_log;
 
void timer_handler(int signum)
{
	log_cpu_utilization = 1;
}

void thread_cleanup_handler(int signum)
{
	if(signum == SIGUSR1)
	{
		printf("\nIndia \n");
		close(logFile[1]);
		pthread_cancel(threads[0]);
                printf("\nIndia \n");

	}
	else if(signum == SIGUSR2)
	{
                printf("\nIndia \n");
		close(logFile[2]);
		pthread_cancel(threads[1]);
                printf("\nIndia \n");

	}

}


void * file_parsing_thread(void *init_info)
{
	//int logFile;
	char buffer[1000];
        struct sigaction sa;
 
        sa.sa_handler = &thread_cleanup_handler;
        sigaction(SIGUSR1, &sa, NULL);

	bzero(buffer, sizeof(buffer));

	logFile[1] = open( ( (info *)init_info )->fileName, O_WRONLY | O_APPEND);
  
        if(logFile[1] == -1)
        { 
                perror("In thread 1:Error opening log file"); 

        }

        print_timestamp(logFile[1]);
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "I am in the File Parsing Thread\n");
        print_to_log_file(logFile[1], buffer, strlen(buffer));

	close(logFile[1]);

	while(1)
	{
		//printf("wbdahckhdbwjdeqwhjbwdqc\n");
		//fflush(stdout);
	}

	pthread_exit(0);

}

void * cpu_utilization_logger_thread(void *init_info)
{

        //int logFile;
        char buffer[1000];

	timer_t timerid;
	struct sigevent timer_signal;
	struct itimerspec timer_info;
	struct sigaction sa;

        struct sigaction sa1;

        sa1.sa_handler = &thread_cleanup_handler;
        sigaction(SIGUSR2, &sa1, NULL);


	sa.sa_handler = &timer_handler;
	sigaction(SIGVTALRM, &sa, NULL);

	(timer_info.it_value).tv_nsec = 100*(1000000);
        (timer_info.it_value).tv_sec = 0;

	(timer_info.it_interval).tv_nsec = 100*(1000000);
        (timer_info.it_interval).tv_sec = 0;


	timer_signal.sigev_notify = SIGEV_SIGNAL;
	timer_signal.sigev_signo = SIGVTALRM;
	timer_signal.sigev_value.sival_ptr = &timerid;


	timer_create(CLOCK_REALTIME, &timer_signal, &timerid);

	timer_settime(timerid, 0, &timer_info, NULL);


        logFile[2] = open( ( (info *)init_info )->fileName, O_WRONLY | O_APPEND);


 
        if(logFile[2] == -1)
        { 
                perror("In thread 2: Error opening log file"); 

        }
	print_timestamp(logFile[2]);
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "I am in the CPU Utilization Logger Thread\n");
        print_to_log_file(logFile[2], buffer, strlen(buffer));

	while(1)
	{
		//	printf("\nbdehwhjdbexwjh\n");
		if(log_cpu_utilization)
		{
			while(flock(logFile[2], LOCK_EX|LOCK_NB));
  			bzero(buffer, sizeof(buffer));
          		sprintf(buffer, "\nLogging CPU Utilization Statisticsi\n");
          		write(logFile[2], buffer, strlen(buffer));
			flock(logFile[2], LOCK_UN);
			logging_cpu_utilization(logFile[2]);
			log_cpu_utilization = 0;
		}
	}

	close(logFile[2]);

	pthread_exit(0);


}

void logging_cpu_utilization(int logFile)
{
	int statFile;
	ssize_t readLength;
	char filePath[20];
	char buffer[100];

	time_t current_time;


	bzero(filePath, sizeof(filePath));
        bzero(buffer, sizeof(buffer));

	sprintf(filePath, "/proc/stat");

	statFile = open(filePath, O_RDONLY);

        if(statFile == -1)
        {
                perror("In thread 2: Error opening /proc/stat file");

        }

	//print_to_log_file(logFile, "\n\n\n\n\n", 4);
	print_timestamp(logFile);
	while(readLength = read(statFile, buffer, sizeof(buffer) ) )
	{
		print_to_log_file(logFile, buffer, readLength);
	}	
        //print_to_log_file(logFile, "\n\n\n\n\n", 4);
	close(statFile);


}

void print_to_log_file(int logFile, char *buffer, ssize_t readLength)
{
        pthread_mutex_lock(&mutex_log);
        write(logFile, buffer, readLength);
	pthread_mutex_unlock(&mutex_log);

}
void print_timestamp(int logFile)
{
	char buffer[200];
	time_t current_time;
	pid_t linuxThreadId;
	pthread_t posixThreadId;

	linuxThreadId = syscall(SYS_gettid); 
	posixThreadId = pthread_self();

        time(&current_time);
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "\nTime: %sPOSIX Thread Id: %d\tLinux Thread id:%d\n", ctime(&current_time), (signed int ) posixThreadId, (signed int ) linuxThreadId);
        print_to_log_file(logFile, buffer, strlen(buffer));
}

int main(int argc, char *argv[])
{

	info init_info;
	//int logFile;
	//pthread_t threads[2];
	char buffer[1000];
	size_t fileNameSize;
	struct sigaction sa, sa1;

	pthread_mutex_init(&mutex_log, NULL);


	if(argc != 2)
	{
		printf("You should pass exactly 1 command line argumen.\n For eg. [program name] [file name]");
		exit(1);
	}

	fileNameSize = strlen(argv[1]);

	init_info.fileName = malloc(fileNameSize);

        bzero(init_info.fileName, fileNameSize);
	strcpy(init_info.fileName, argv[1]);

	printf("\n%d\n", getpid());
       
	
	logFile[0] = open(init_info.fileName, O_CREAT|O_TRUNC|O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
	printf("Filename:%s File Desc:%d", init_info.fileName, logFile[0]);
	fflush(stdout);
	

	if(logFile[0]<0)
	{
		perror("In main thread Error opening log file"); 
		
	}	

        print_timestamp(logFile[0]);
        sprintf(buffer, "Main Thread has begun execution\n");
        print_to_log_file(logFile[0], buffer, strlen(buffer));

	bzero(buffer, sizeof(buffer));


	if(pthread_create(&threads[0], NULL, file_parsing_thread, &init_info))
	{
		perror("Error creating a file_parsing_thread thread");
		exit(1);
	}


	if(pthread_create(&threads[1], NULL, cpu_utilization_logger_thread, &init_info))
	{
                perror("Error creating a cpu_utilization_logger_thread");
                exit(1);
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	exit(1);

}


