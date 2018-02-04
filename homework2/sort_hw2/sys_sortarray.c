#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE3(sort_array, int32_t __user *, src, int __user, sort_size,int32_t __user *, dst)
{
        int buffer_size, num_elements, temp, i, j;
        int32_t *b;
        size_t size_int, size_int32_t;

	printk(KERN_INFO "Entered the System Call 'sort_array'. This system call sorts an array of integers using Selection Sort Technique\n");
	printl(KERN_INFO "The size of the array/buffer to be sorted is %d\n", sort_size);

        size_int = sizeof(int);
        size_int32_t = sizeof(int32_t);

	buffer_size = sort_size;

        b = (int32_t *) kmalloc((size_t)buffer_size, GFP_KERNEL);

        if(copy_from_user(b, src, (unsigned long)buffer_size))
        {
                printk(KERN_INFO "Source Buffer pointer (src) is invalid. \n");
		kfree(b);
                return -EFAULT;
        }

        num_elements = buffer_size/size_int32_t;

        printk(KERN_INFO "Given Array \n");
        for(i = 0; i<num_elements; i++)
        {
                printk(KERN_INFO "b[%d] = %d \t", i, *(b + i));
        }

        printk(KERN_INFO "\n");

	printk(KERN_INFO "Starting Selection sort \n")

        for(i = 0; i<num_elements; i++)
        {
                for(j = i; j<num_elements; j++)
                {
                        if(*(b + j) > *(b + i))
                        {
                                temp = *(b + j);
                                *(b + j) = *(b + i);
                                *(b + i) = temp;
                        }

                }
        }
	
	printk(KERN_INFO "Finished Selection Sort \n");

        printk(KERN_INFO "Sorted Array\n");

        for(i = 0; i<num_elements; i++)
        {
                printk(KERN_INFO "b[%d] = %d \t", i, *(b+i));
        }
        printk(KERN_INFO "\n");

        if(copy_to_user(dst, b, (unsigned long)buffer_size))
        {
                printk(KERN_INFO "Destination Buffer pointer (dst) is invalid. \n");
		kfree(b);
                return -EFAULT;
        }
	kfree(b);
	return 0;
}

