#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/string.h>
#include<linux/kthread.h>
#include<linux/kfifo.h>


#define AUTHOR "Prithvi Teja Veeravalli"
#define LICENSE "GPL"
#define DESC "This kernel module generates 2 kthreads where the 1st kthread sends currently scheduled processes in rbtree to the 2nd kthread using a kfifo. 2nd kthread logs that information"

#define FIFO_SIZE 1024

static DECLARE_KFIFO(test, unsigned char, FIFO_SIZE);


static __init int init_rbtree_logging_module(void);  //Declaring the init module
static __exit void clean_rbtree_logging_module(void);   // Declaring the cleanup module

static struct task_struct *kthread1, *kthread2;

static int rbtree_threadfn(void *data);      //Thread fn for the kthread which reads scheeduled processes using rbtree and sends it to logging_threadfn using kfifo.
static int logging_threadfn(void *data);      //Thread fn for teh kthread which logss data on the kfifo

extern struct task_struct init_task;

//static struct kfifo rbtree_log_queue;
//static int retval = 5;

MODULE_AUTHOR(AUTHOR);
MODULE_LICENSE(LICENSE);
MODULE_DESCRIPTION(DESC);
MODULE_SUPPORTED_DEVICE("Common - This module is expected to work across all architectures.");

module_init(init_rbtree_logging_module);
module_exit(clean_rbtree_logging_module);

static int rbtree_threadfn(void *data)
{
	printk(KERN_INFO "I am in the 'rbtree scheduled processes' thread fn.\n\n");
	while(!kthread_should_stop());

	kthread1 = NULL;
	return 0;
}

static int logging_threadfn(void *data)
{
        printk(KERN_INFO "I am in the 'logging_threadfn' thread fn.\n\n");
	while(!kthread_should_stop());
	kthread2 = NULL;
        return 0;
}

static __init int init_rbtree_logging_module(void)
{
        printk(KERN_INFO "Loading the kernel module 'rbtree_logging'.\n");

	INIT_KFIFO(test);

	//kfifo_alloc(&rbtree_log_queue, 2, GFP_KERNEL);
/*	if(!kfifo_alloc(&rbtree_log_queue, PAGE_SIZE, GFP_KERNEL))
	{*/
		kthread1 = kthread_create(rbtree_threadfn, NULL, "rbtree scheduled processes");
        	if (!IS_ERR(kthread1))
        	{
                	printk(KERN_INFO "Created a thread for 'rbtree scheduled processes'.\n");
                	wake_up_process(kthread1);
        	}
        	else
                	printk(KERN_INFO "Error creating a thread for 'rbtree scheduled processes'.\n");
                kthread2 = kthread_create(logging_threadfn, NULL, "Logging Thread");
                if (!IS_ERR(kthread2))
                {
                        printk(KERN_INFO "Created a thread for 'Logging kfifo'.\n");
                        wake_up_process(kthread2);
                }
                else
                        printk(KERN_INFO "Error creating a thread for 'Logging kfifo'.\n");

//	}
//	else
//	{
		//retval = 5;
//	}
	return 0;		
}

static __exit void clean_rbtree_logging_module(void)
{


        printk(KERN_INFO "Unloading the kernel module 'process_tree_traverse'.\n");

        if (kthread1)
        {
                kthread_stop(kthread1);
                printk(KERN_INFO "Finished stopping 'rbtree scheduled processes'.\n");
        }
        else
                printk(KERN_INFO "'rbtree scheduled processes' doesn't exist. It might have already been closed\n");

        if (kthread2)
        {
                kthread_stop(kthread2);
                printk(KERN_INFO "Finished stopping 'Logging kfifo'.\n");
        }
        else
                printk(KERN_INFO "'Logging kfifo' doesn't exist. It might have already been closed\n");

        
  	kfifo_free(&test);


}


