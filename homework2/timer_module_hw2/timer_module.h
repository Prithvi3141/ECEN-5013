#ifndef _TIMER_MODULE_H
#define _TIMER_MODULE_H

#include <linux/timer.h>

struct timer_list mytimer1, mytimer2;

static __init int init_timer_module(void);
static __init void clean_timer_module(void);

void timer_handler(unsigned long data);

#endif



