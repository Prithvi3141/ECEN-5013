#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

#define BUFFER_LEN 100
#define CHAR_LEN 1

int main()
{
	FILE *file;
	char hw_string[10] = "Homework1";
	char fileName[15] = "Test_File_HW1";
	char *fileBuffer;
	size_t len;
	char c = 'T';

	printf("This program does File Operations required for %s", hw_string);
	file = fopen(fileName, "w");
	if(!file)
	{
		printf("Error opening file");
		return 1;
	}
	chmod(fileName, S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH);
	

	//Change made in the below line
	fputc((int) c, file);
	fclose(file);
        file = fopen(fileName, "a");

        if(!file)
        {
                printf("Error opening file");
                return 1;
        }


        fileBuffer = malloc(BUFFER_LEN);
        bzero(fileBuffer, BUFFER_LEN);
	
	fgets(fileBuffer, BUFFER_LEN, stdin);
	
	len = strlen(fileBuffer);
	fwrite(fileBuffer, CHAR_LEN, len, file);
	fflush(file);
        fclose(file);
        file = fopen(fileName, "r");

        if(!file)
        {
                printf("Error opening file");
                return 1;
        }


        bzero(fileBuffer, BUFFER_LEN);
	*fileBuffer = (char)fgetc(file);
	fgets(fileBuffer, BUFFER_LEN, file);
	fclose(file);
	free(fileBuffer);




	
        //bzero(fileBuffer, BUFFER_LEN);
	//strcpy(fileBuffer, "This is my first write in to a file.\n");
	//len = strlen(fileBuffer);
	//fwrite(file, CHAR_LEN, len, file);

		
	

	return 0;	
}

