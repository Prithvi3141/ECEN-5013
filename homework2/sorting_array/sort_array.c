#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
//#include<sys/syscall.h> 
//#include<linux/kernel.h>
#include<unistd.h>

#define BUFFER_SIZE 256 

int main()
{
	int num_elements, temp;
	int smallest_element;
	int *sorted_array;
	int buffer_size = 40;
	char buff = 40;
	int32_t a[10];

	int32_t b[10]; //= {82, 35, 42, 91, 93, 44, 132, 258, 110, 11};

//        sorted_array = malloc(buffer_size*sizeof(int));

        num_elements = buffer_size/sizeof(int32_t);

	printf("Given Array %d\n", (int )sizeof(int32_t));
        for(int i = 0; i<num_elements; i++)
        {
		b[i] = rand();
                printf("b[%d] = %d \t", i, b[i]);       
        }

	printf("\n");

	syscall(333, b, buffer_size, a);

//	for(int i = 0; i<num_elements; i++)
//	{
//		for(int j = i; j<num_elements; j++)
//		{
//			if(b[j]>b[i])
//			{
//				temp = b[j];
//				b[j] = b[i];
//				b[i] = temp;	
//			}
//			
//		}
//	}


	
	printf("Sorted Array\n");
	for(int i = 0; i<num_elements; i++)
	{
		printf("a[%d] = %d \t", i, a[i]);	
	}
	printf("\n");


	return 1;
}
