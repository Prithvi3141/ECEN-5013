/*
# File_Name: file_operation.c
# Author:  Prithvi Teja Veeravalli <prithvi.veeravalli.colorado.edu>
# Description: This file is used to perform a bunch of file operations listed as part of the homework and to analyse the program by using tools such as strace, ltrace and perf.
# Version:1.2
*/
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

	printf("This program does File Operations required for %s", hw_string);  //Printing to standard output
	file = fopen(fileName, "w"); // Creating a  file
	if(!file)
	{
		printf("Error opening file");
		return 1;
	}
	chmod(fileName, S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH); // Modifying the permissions of teh file to read and write by owner, group and others
	

	//Change made in the below line
	fputc((int) c, file);       //Write character to the file
	fclose(file);               //Close the file
        file = fopen(fileName, "a");   //Open the file in append mode

        if(!file)
        {
                printf("Error opening file");
                return 1;
        }


        fileBuffer = malloc(BUFFER_LEN);       //Dynamically allocating area of memory
        bzero(fileBuffer, BUFFER_LEN);
	
	fgets(fileBuffer, BUFFER_LEN, stdin);    // Reading an input string from command line
	
	len = strlen(fileBuffer);
	fwrite(fileBuffer, CHAR_LEN, len, file);   //Writing inputted string to command line
	fflush(file);                              // Flush file output
        fclose(file);                              //Closing the file
        file = fopen(fileName, "r");               // Open File for reading

        if(!file)
        {
                printf("Error opening file");
                return 1;
        }


        bzero(fileBuffer, BUFFER_LEN);
	*fileBuffer = (char)fgetc(file);            // Reading a single character
	fgets(fileBuffer, BUFFER_LEN, file);         // Reading a string of characters
	fclose(file);				    // close the file
	free(fileBuffer);			    // Freeing memory

	return 0;	
}

