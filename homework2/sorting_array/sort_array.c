#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
//#include<sys/syscall.h> 
//#include<linux/kernel.h>
#include<unistd.h>

#define BUFFER_SIZE 256 

int main()
{
	int temp, smallest_element, seed = 1;
	int *sorted_array;
	int num_elements = 256;
	int buffer_size = num_elements*sizeof(int32_t);
	char buff = 40;
	char *c;
	int32_t a[num_elements];

	int32_t b[num_elements]; //= {82, 35, 42, 91, 93, 44, 132, 258, 110, 11};

        num_elements = buffer_size/sizeof(int32_t);

	printf("******************************************************************Given Array*****************************************************\n");
        for(int i = 0; i<num_elements; i++)
        {
		b[i] = ( (int32_t) rand_r(&seed))%100;
                printf("%d \t", b[i]);       
        }

	printf("\n\n\n\n");

	if(syscall(333, b, buffer_size, a))
	{
		perror("The parameters passed to the system call are invalid");
		return 1;
	}
	
	printf("*******************************************************************Sorted Array*****************************************************\n");
	for(int i = 0; i<num_elements; i++)
	{
		printf("%d \t \t", a[i]);	
	}
	printf("\n");


	return 0;
}
