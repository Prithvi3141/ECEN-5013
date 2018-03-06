#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/string.h>
#include<linux/kthread.h>


#define AUTHOR "Prithvi Teja Veeravalli"
#define LICENSE "GPL"
#define DESC "This kernel module generates a kthread which traverses through process tree lineage and print some metrics"

static __init int init_process_tree_traverse_module(void);  //Declaring the init module
static __exit void clean_process_tree_traverse_module(void);   // Declaring the cleanup module

static struct task_struct *kthread1;

static int process_tree_log_metrics(void *data);      //Thread fn for the kthread created by the module

extern struct task_struct init_task;
 
MODULE_AUTHOR(AUTHOR);
MODULE_LICENSE(LICENSE);
MODULE_DESCRIPTION(DESC);
MODULE_SUPPORTED_DEVICE("Common - This module is expected to work across all architectures.");


/* This is the definition of the callback handler of kernel timer*/
static int process_tree_log_metrics(void *data)
{
	struct task_struct *traverse;
	struct list_head *list;

	int children_count = 0;

	traverse = current;
        printk(KERN_INFO "I am traversing through the Process Tree. \n");
	while(!kthread_should_stop())
	{
		printk(KERN_INFO "Thread Name:%s\n", traverse->comm);
		printk(KERN_INFO "Process id:%d\n", traverse->pid);
		printk(KERN_INFO "Process Status:%d\n", traverse->state);
		list_for_each(list, &traverse->children)
		{
			children_count++;
		}
		printk(KERN_INFO "Number of Children:%d\n", children_count);
		children_count=0;
                printk(KERN_INFO "Nice Values:%d\n\n\n\n", (traverse->rt_priority - 120));	
		if(traverse != &init_task)
			traverse = traverse->parent;
		else
			break;
		//printk(KERN_INFO "I am traversing through the Process Tree. \n");
	}
	do_exit(0);
	return 0;
}
  /*This is the definition of the entry or init function*/
static __init int init_process_tree_traverse_module(void)
{

        printk(KERN_INFO "Loading the kernel module 'process_tree_traverse'.\n");

	//kthread1 = kmalloc(sizeof(struct task_struct), GFP_KERNEL);
	
	kthread1 = kthread_create(process_tree_log_metrics, NULL, "Process Tree Traversal Thread");
	if (!IS_ERR(kthread1))
	{
		printk(KERN_INFO "Created a thread for Process Tree Traversal & logging.\n");
                wake_up_process(kthread1);
	}
	else
		printk(KERN_INFO "Error creating a thread for Process Tree Traversal & Logging.\n");
	

        return 0;
}

static __exit void clean_process_tree_traverse_module(void)
{
	printk(KERN_INFO "Unloading the kernel module 'process_tree_traverse'.\n");
	if (!IS_ERR(kthread1))
	{
		kthread_stop(kthread1);
		printk(KERN_INFO "Finished stopping 'Process Tree Traversal & logging thread'.\n");
	}
	else
		printk(KERN_INFO "'Process Tree Traversal & Logging Thread' doesn't exist.\n");
}

module_init(init_process_tree_traverse_module);
module_exit(clean_process_tree_traverse_module);

