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
	char data;
	int count;
	//unsigned long makki;
	struct node list_link;
};


#define GET_LIST_CONTAINER(addr, type, member) ({\
	const __typeof__( ( (type * )0)->member) *__ptr = (addr);\
	(type *) (( (char *) __ptr ) - offsetof(type, member)); }\
)
/*This function is used to destroy a linked list being pointed by the head_list node. This function returns a struct node * type*/
struct node * destroy(struct node *head_list);
/*This function is used to create a new element containing data parameter as data and it should be placed at the beginning of the linked list. The function passes head_list node which points to the linked list to which the element should be inserted at the beginning list*/
struct node * insert_at_beginning(struct node *head_list, char data);
/*This function is used to create a new element containing data parameter as data and it should be placed at the end of the linked list. The function passes head_list node which points to the linked list to which the element should be inserted at the end of the list*/
struct node * insert_at_end(struct node *head_list, char data);
/*This function is used to create a new element containing data parameter as data and it should be placed at the position mentioned by the position parameter. The function passes head_list node which points to the linked list to which the element should be inserted at the specified position*/
struct node * insert_at_position(struct node *head_list, size_t position, char data);
/*This function is used to delete an element at the beginning of the list and make the 2nd element on the linked list as the new head node i.e. make the head_list->next point the 2nd element of the list if any. If there is no 2nd element the head_list->next is null*/
struct node * delete_at_beginning(struct node *head_list);
/*This function is used to delete an element at the end of the list and make the element preceding the last element have a next pointer of null*/
struct node * delete_at_end(struct node *head_list);
/*This function is used to delete an element at the specified position of the list*/
struct node * delete_at_position(struct node *head_list, size_t position);
/*This function returns the size of the linked list in size_t datatype*/
size_t size(struct node *head_list);
char get_data(struct node *head_list, size_t position);
int increment_count(struct node *head_list, size_t position);





#endif
