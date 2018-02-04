# CRON TASK 

This subdirectory contains periodic_cron_task.c which has been scheduled by cron to execute every 10 minutes and log the output to cron_log file.

This subdirectory has the following files.

1) periodic_cron_task.c - This c program uses the sort_array system call and a few other system calls. 

2) prithvi-crontab - This is the crontab file that has been used to schedule a daemon thread which executes periodic_cron_task.c every 10 Minutes.

3) cron-log - This logs the output of the cron task/job which executes periodic_cron_task.c.
