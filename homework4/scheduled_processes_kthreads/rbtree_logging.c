#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/timer.h>
#include <linux/kthread.h>  
#include <linux/sched.h>  
#include <linux/time.h>   
#include <linux/proc_fs.h>
#include <linux/kfifo.h>
#include <linux/hrtimer.h>
#include <linux/delay.h> 
#include <linux/err.h>
#include <linux/irq.h>
#include <linux/clk.h>
#include <linux/list.h>
#include <linux/rtmutex.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/string.h>

#define AUTHOR "Prithvi Teja Veeravalli"
#define LICENSE "GPL"
#define DESC "This kernel module generates a kthread which traverses through process tree lineage and print some metrics"
#define FIFO_SIZE  4096

static struct task_struct *kthread1;
static struct task_struct *kthread2;
  
static DECLARE_KFIFO(msg_queue, char, FIFO_SIZE);
struct timer_list g_timer;


MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);


static int rbtree_threadfn(void * data) 
{
        struct task_struct *prev_task, *next_task, *traverse;
	char buffer[200];
	ktime_t timeout;
	

	printk(KERN_INFO "Schedule rbtree thread has been spawned.\n");
	while(!kthread_should_stop())
	{

		traverse = current;

                memset(buffer, 0, sizeof(buffer));


		prev_task= list_entry(traverse->tasks.prev, struct task_struct, tasks);
		next_task=list_entry(traverse->tasks.next, struct task_struct, tasks);

		sprintf(buffer, "\tCurrent Task:\n Name: %s\nPID: %d\nvruntime: %llu\n\n\n\n", (traverse->comm), (traverse->pid), traverse->se.vruntime);

        	 sprintf(buffer, "%s\tPrevious Task:\n Name: %s\nPID: %d\nvruntime: %llu\n\n\n\n", buffer, (prev_task->comm), (prev_task->pid), prev_task->se.vruntime);

	         sprintf(buffer, "%s\tNext Task:\n Name: %s\nPID: %d\nvruntime: %llu\n\n\n\n", buffer, (next_task->comm), (next_task->pid), next_task->se.vruntime);

		kfifo_in(&msg_queue, buffer, strlen(buffer));

		timeout = ktime_get();
		timeout = ktime_add_us(timeout, 1000000);
		__set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_hrtimeout_range(&timeout, 100, HRTIMER_MODE_ABS);
	}
	
	do_exit(0);
	return 0;
}

static int logging_threadfn(void * data) 
{
	unsigned char buffer[200];

	printk(KERN_INFO "Logging thread has been spawned.\n");

	while(!kthread_should_stop())
	{
		if(kfifo_len(&msg_queue) > 0)
		{
			memset(buffer, 0, sizeof(buffer));
			kfifo_out(&msg_queue, buffer, 200);	
			printk(KERN_INFO "%s", buffer);	
		}	
	}

	do_exit(0);
	return 0;
}

static __init int init_schedule_rbtree_module(void)
{	
	
	printk(KERN_INFO "Loading the kernel module 'kthreads & kfifo'.\n");
	
	INIT_KFIFO(msg_queue);

	kthread1 = kthread_create(rbtree_threadfn, NULL, "Schedule Processing Thread");
	if (IS_ERR(kthread1)) {
		printk(KERN_ERR "Error Creating thread\n");
		return -ESRCH;
	}
	
	wake_up_process(kthread1);
	

	kthread2 = kthread_create(logging_threadfn,NULL, "Logging Thread");
	if (IS_ERR(kthread2)) {
		printk(KERN_ERR "Error creating thread\n");
		return -ESRCH;
	}
	printk(KERN_INFO "Second thread created");
	wake_up_process(kthread2);
	

	return 0;
}

static __exit void clean_schedule_rbtree_module(void)
{	
        printk(KERN_INFO "Unloading the kernel module 'kthreads & kfifo'.\n");

        if (kthread1)
        {
                kthread_stop(kthread1);
                printk(KERN_INFO "Finished stopping 'Scheduled rbtree thread''.\n");
        }
        else
                printk(KERN_INFO "'Schedule rbtree thread Thread' doesn't exist. It might have already been closed\n");

        if (kthread2)
        {
                kthread_stop(kthread2);
                printk(KERN_INFO "Finished stopping 'logging thread'.\n");
        }
        else
                printk(KERN_INFO "'Logging Thread' doesn't exist. It might have already been closed\n");


	kfifo_free(&msg_queue);
}

module_init(init_schedule_rbtree_module);
module_exit(clean_schedule_rbtree_module);
