#ifndef _DOUBLE_L_LIST_H
#define _DOUBLE_L_LIST_H

#include <stddef.h>
#include <stdint.h>

struct node
{
	struct node *next;
	struct node *prev;
};

struct int_list
{
	uint32_t data;
	//unsigned long makki;
	struct node list_link;
};


#define GET_LIST_CONTAINER(addr, type, member) ({\
	const typeof( ( (type * )0)->member) *__ptr = (addr);\
	(type *) (( (char *) __ptr ) - offsetof(type, member)); }\
)

struct node * insert_at_beginning(struct node *head_list, uint32_t data);
struct node * insert_at_end(struct node *head_list, uint32_t data);
struct node * insert_at_position(struct node *head_list, size_t position, uint32_t data);
size_t size(struct node *head_list);



#endif
