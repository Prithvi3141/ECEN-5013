/*
# File_Name: double_l_list.c
# Author:  Prithvi Teja Veeravalli <prithvi.veeravalli.colorado.edu>
# Description: This library is used to perform function of a doubly linked lists.
# Tools used: bash shell is used to interpret this script 

*/
#include <stdio.h>
#include "double_l_list.h"
#include<stdint.h>
#include<stdlib.h>

/*This function is used to destroy a linked list being pointed by the head_list node. This function returns a struct node * type*/
struct node * destroy(struct node *head_list)
{
	struct node *element_node, *next_element_node;
	struct int_list *element;

	if(head_list->next == NULL)
	{
		printf("The linked list is empty and there is no need to destroy. \n");
		return NULL;
	}
	else
	{
		element_node = head_list->next;
		head_list->next = NULL;       //making the head_list->next pointer null indicating that a linked list is empty or has not been created yet
		/*Iterating through every element in the linked list and freeing the memory by deleting every element*/
		while(element_node)
		{
			next_element_node = element_node->next;
        		element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
        	        free(element);
			element_node = next_element_node;
		}
		return head_list;
	}

}
/*This function is used to create a new element containing data parameter as data and it should be placed at the beginning of the linked list. The function passes head_list node which points to the linked list to which the element should be inserted at the beginning list*/
struct node * insert_at_beginning(struct node *head_list, uint32_t data)
{
	struct int_list *element;
	struct node *sec_element;

        element = (struct int_list *)malloc(sizeof(struct int_list));

//Adding element to the beginning of the list if there is no element
	if(head_list->next == NULL)
	{
		head_list->next = &(element->list_link);
		(element->list_link).next = NULL;	
	}
//Adding an elemenyt to the list if there is one or more elements present
	else
	{
		(element->list_link).next = head_list->next;
		head_list->next = &(element->list_link);
		sec_element = (element->list_link).next;
		sec_element->prev = &(element->list_link);
	}
	element->data = data;
	(element->list_link).prev = NULL;

	return head_list;
}

/*This function is used to create a new element containing data parameter as data and it should be placed at the end of the linked list. The function passes head_list node which points to the linked list to which the element should be inserted at the end of the list*/
struct node * insert_at_end(struct node *head_list, uint32_t data)
{
	struct int_list *element;
	struct node *element_node;

	element = (struct int_list *)malloc(sizeof(struct int_list));

//Adding an element if there is no element present in the linked list
        if(head_list->next == NULL)
	{
		head_list->next = &(element->list_link);
		(element->list_link).prev = NULL;
	}
//Adding an element if there is one or more element present in the linked list
	else
	{
		element_node = head_list->next;
		while(element_node->next)
		{
			element_node = element_node->next;
		}
		element_node->next = &(element->list_link);
		(element->list_link).prev = element_node;
	}
	element->data = data;
	(element->list_link).next = NULL;

	return head_list;
}
/*This function is used to create a new element containing data parameter as data and it should be placed at the position mentioned by the position parameter. The function passes head_list node which points to the linked list to which the element should be inserted at the specified position*/
struct node * insert_at_position(struct node *head_list, size_t position, uint32_t data)
{
	struct int_list *element;
	struct node *element_node;
	struct node *prev_element_node;

	element = (struct int_list *)malloc(sizeof(struct int_list));

//If the position parameter provided is 0. The funcction returns because there is no such position as 0
	if(position == 0)
	{
		printf("Invalid position. Positions should begin from 1.\n");
		return NULL;
	}
//Adduing aan element if there is no element
        if(head_list->next == NULL)
        {
                head_list->next = &(element->list_link); 
                (element->list_link).prev = NULL;
		(element->list_link).next = NULL;
        }
	else
	{
		if(size(head_list) >= position)
		{
			int i = 1;

		        element_node = head_list->next;

			//Traversing through nodes to get to the required position
			while(i<position)
			{
				element_node = element_node->next;
				i++;
			}
			if(i>1)
			{
				prev_element_node = element_node->prev;
				prev_element_node->next = &(element->list_link);
				element_node->prev = &(element->list_link);
				(element->list_link).next = element_node;
				(element->list_link).prev = prev_element_node;
			}
			else
			{
		        	element_node->prev = &(element->list_link);
				(element->list_link).next = element_node;
                                (element->list_link).prev = NULL;
				head_list->next = &(element->list_link); 

			}
			

				
		}
		//If size of the linked list is less than the position specified element is being placed at the end of the linked list
		else
		{
	                element_node = head_list->next;
        	        while(element_node->next)
        	        {
        	                element_node = element_node->next;
        	        }
        	        element_node->next = &(element->list_link);
        	        (element->list_link).prev = element_node;
		        (element->list_link).next = NULL;

		}

		element->data = data;
	}
	
	return head_list;
} 
/*This function is used to delete an element at the beginning of the list and make the 2nd element on the linked list as the new head node i.e. make the head_list->next point the 2nd element of the list if any. If there is no 2nd element the head_list->next is null*/
struct node * delete_at_beginning(struct node *head_list)
{
	struct node *element_node, *next_element_node;
	struct int_list *element;	

//If head pointers next pointer is pointing to null there is no element in the linked list to delete
	if(head_list->next == NULL)
	{
		return NULL;
	}
	else
	{

		element_node = head_list->next;
		next_element_node = element_node->next;
		//If there is one or more elements in the linked list
		if(next_element_node)
		{
			head_list->next = next_element_node;
			next_element_node->prev = NULL;
			element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
			free(element);
		}
		// If there is one element in the linked list
		else
		{
			head_list->next = NULL;
			element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
			free(element);
		}
		return head_list;
	}
}
/*This function is used to delete an element at the end of the list and make the element preceding the last element have a next pointer of null*/
struct node * delete_at_end(struct node *head_list)
{
        struct node *element_node, *next_element_node;
        struct int_list *element;
//If head pointers next pointer is pointing to null there is no element in the linked list to delete

        if(head_list->next == NULL)
        {
                return NULL;
        }
        else
        {
		element_node = head_list->next;
                next_element_node = element_node->next;
		//If there is one or more elements in the linked list

		if(next_element_node)
		{
                	while(element_node->next)
                	{
                	        element_node = element_node->next;
                	}
			(element_node->prev)->next = NULL;
			element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
			free(element);
		}
                // If there is one element in the linked list
		else
		{
                        head_list->next = NULL;
                        element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
                        free(element);
		}

		return head_list;				
	}


}

struct node * delete_at_position(struct node *head_list, size_t position)
{
        struct int_list *element;
        struct node *element_node;
        struct node *prev_element_node;
	
	//There is no such position as position 0 in a linked list. Position argument in this library aaccepts positions from 1 or more
        if(position == 0)
        {
                printf("Invalid position. Positions should begin from 1.\n");
                return NULL;
        }
	//If head pointers next pointer is pointing to null there is no element in the linked list to delete

        if(head_list->next == NULL)
        {
        	printf("Can't delete an empty linked list \n");
		return NULL;
        }
	else
	{
                if(size(head_list) >= position)
		{
                        int i = 1;

                        element_node = head_list->next;

                        while(i<position)
                        {
                                element_node = element_node->next;
                                i++;
                        }
                        if(i>1)
                        {
				if(size(head_list) == position)
				{
			                (element_node->prev)->next = NULL;
		                        element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
                        		free(element);

				}
				else
				{
					prev_element_node = element_node->prev;
					prev_element_node->next = element_node->next;
					(element_node->next)->prev = prev_element_node;
	                                element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
         	                        free(element);

				}	
                                /*prev_element_node = element_node->prev;
                                prev_element_node->next = &(element->list_link);
                                element_node->prev = &(element->list_link);
                                (element->list_link).next = element_node;
                                (element->list_link).prev = prev_element_node;*/
                        }
                        else
                        {
	                        head_list->next = NULL;
        	                element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
        	                free(element);
                        }
			return head_list;

		}
		else
		{
	                printf("Delete Failed: Since the position %lu  is greater than the size of the linked list %lu\n", position, size(head_list));
        	        return head_list;
		}

	}

}
/*This function returns the size of the linked list in size_t datatype*/
size_t size(struct node *head_list)
{
	size_t size = 0;
	struct node *element_node;
	
	//If head_list's next pointer points to null the size of the linked list is zero.
	if(head_list->next == NULL)
	{
		return size;	
	}
	else
	{
		element_node = head_list->next;
		size++;
		while(element_node->next)
		{
			element_node = element_node->next;
			size++;
		}
		return size;
	}
}

uint32_t get_data(struct node *head_list, size_t position)
{
        struct int_list *element;
        struct node *element_node;

        element_node = head_list->next;

        int i = 1;

        element_node = head_list->next;

        while(i<position)
        {
                element_node = element_node->next;
                i++;
        }
        element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);

        return (element->data) ;


}

