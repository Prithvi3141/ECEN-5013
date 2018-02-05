/*
  # File_Name: build_kernel
  # Author:  Prithvi Teja Veeravalli <prithvi.veeravalli.colorado.edu>
  # Description: This kernel module registers a kernel timer to wake up every count_time(500ms by defaul  t) which wakes up the timer_handler(callback for the kernel timer) and logs some information
  # Version: 1.3

  */
  #include<linux/module.h>
  #include<linux/kernel.h>
  #include<linux/init.h>
  #include<linux/moduleparam.h>
  #include <linux/timer.h>
  #include <linux/string.h>

  #define AUTHOR "Prithvi Teja Veeravalli"
  #define LICENSE "GPL"
  #define DESC "This kernel module registers a kernel timer to wake up every 500msec which logs some inf  ormation"

  static char *user_name = "Prithvi Teja Veeravalli";
  static unsigned long  count_time = 500;

  static struct timer_list mytimer1, mytimer2;

  static __init int init_timer_module(void);  //Declaring the init module
  static __exit void clean_timer_module(void);   // Declaring the cleanup module

  void timer_handler(unsigned long data);      //Declaring the kernel timer callback handler


  MODULE_AUTHOR(AUTHOR);
  MODULE_LICENSE(LICENSE);
  MODULE_DESCRIPTION(DESC);
  MODULE_SUPPORTED_DEVICE("Common - This module is expected to work across all architectures.");

  module_param(user_name, charp, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);    //receiving command line argument   user_name
  module_param(count_time, ulong, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);   //receiving command line argument   coun_time

/* This is the definition of the callback handler of kernel timer*/
  void timer_handler(unsigned long data)
  {
          static int timer_interrupt = 0;
  
          timer_interrupt++;
          printk(KERN_INFO "User: %s The timer1 has fired for %d with count time:%lu", user_name, timer_interrupt, count_time);

          mod_timer(&mytimer1, jiffies + msecs_to_jiffies(count_time));  //Changing the kernel timer expires field to 500msec from now aand activating the timer
  }
  /*This is the definition of the entry or init function*/
  static __init int init_timer_module(void)
  {

          printk(KERN_INFO "Loading the kernel module 'timer_module'.\n");

          init_timer(&mytimer1);        //Initialising the struct timer_list structure element and adding a kernel timer but not activating it yet

          setup_timer(&mytimer1, timer_handler, 0);     //registering the callback handler to kernel timer

          mytimer1.expires = jiffies + msecs_to_jiffies(count_time);  //setting the expires field
          add_timer(&mytimer1);       //ACtivating the timer

          return 0;
  }

  static __exit void clean_timer_module(void)
  {
          del_timer_sync(&mytimer1);        //Deleting the kernel timer
          printk(KERN_INFO "Unloading the kernel module 'timer_module'.\n");
  }

  module_init(init_timer_module);
  module_exit(clean_timer_module);

