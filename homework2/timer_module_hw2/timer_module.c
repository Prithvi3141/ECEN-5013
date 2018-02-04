#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/moduleparam.h>
//#include "timer_module.h"
#include <linux/timer.h>
#include <linux/string.h>

#define AUTHOR "Prithvi Teja Veeravalli"
#define LICENSE "GPL"
#define DESC "This kernel module registers a kernel timer to wake up every 500msec which logs some information"

static char *user_name = "Prithvi Teja Veeravalli";
static unsigned long  count_time = 500;

static struct timer_list mytimer1, mytimer2;

static __init int init_timer_module(void);
static __exit void clean_timer_module(void);

void timer_handler(unsigned long data);


MODULE_AUTHOR(AUTHOR);
MODULE_LICENSE(LICENSE);
MODULE_DESCRIPTION(DESC);
MODULE_SUPPORTED_DEVICE("Common - This module is expected to work across all architectures.");

module_param(user_name, charp, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
module_param(count_time, ulong, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

void timer_handler(unsigned long data)
{
	static int timer_interrupt = 0;

        timer_interrupt++;
        printk(KERN_INFO "User: %s The timer1 has fired for %d with count time:%lu", user_name, timer_interrupt, count_time);

        mod_timer(&mytimer1, jiffies + msecs_to_jiffies(count_time));
}

static __init int init_timer_module(void)
{

        printk(KERN_INFO "Loading the kernel module 'timer_module'.\n");

	init_timer(&mytimer1);

	setup_timer(&mytimer1, timer_handler, 0);

	mytimer1.expires = jiffies + msecs_to_jiffies(count_time);
	add_timer(&mytimer1);
	
	return 0;
}

static __exit void clean_timer_module(void)
{
	del_timer_sync(&mytimer1);
	printk(KERN_INFO "Unloading the kernel module 'timer_module'.\n");
}

module_init(init_timer_module);
module_exit(clean_timer_module);



