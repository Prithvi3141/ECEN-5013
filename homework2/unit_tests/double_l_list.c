#include <stdio.h>
#include "double_l_list.h"
#include<stdint.h>
#include<stdlib.h>

struct node * destroy(struct node *head_list)
{
	struct node *element_node, *next_element_node;
	struct int_list *element;

	if(head_list->next == NULL)
	{
		printf("The linked list is empty and there is no need to destroy. \n");
		return head_list;
	}
	else
	{
		element_node = head_list->next;
		head_list->next = NULL;
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

struct node * insert_at_beginning(struct node *head_list, uint32_t data)
{
	struct int_list *element;
	struct node *sec_element;

        element = (struct int_list *)malloc(sizeof(struct int_list));

	if(head_list->next == NULL)
	{
		head_list->next = &(element->list_link);
		(element->list_link).next = NULL;	
	}
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

struct node * insert_at_end(struct node *head_list, uint32_t data)
{
	struct int_list *element;
	struct node *element_node;

	element = (struct int_list *)malloc(sizeof(struct int_list));

        if(head_list->next == NULL)
	{
		head_list->next = &(element->list_link);
		(element->list_link).prev = NULL;
	}
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

struct node * insert_at_position(struct node *head_list, size_t position, uint32_t data)
{
	struct int_list *element;
	struct node *element_node;
	struct node *prev_element_node;

	element = (struct int_list *)malloc(sizeof(struct int_list));

	if(position == 0)
	{
		printf("Invalid position. Positions should begin from 1.\n");
		return head_list;
	}

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

struct node * delete_at_beginning(struct node *head_list)
{
	struct node *element_node, *next_element_node;
	struct int_list *element;	

	if(head_list->next == NULL)
	{
		return head_list;
	}
	else
	{

		element_node = head_list->next;
		next_element_node = element_node->next;
		if(next_element_node)
		{
			head_list->next = next_element_node;
			next_element_node->prev = NULL;
			element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
			free(element);
		}
		else
		{
			head_list->next = NULL;
			element = GET_LIST_CONTAINER(element_node, struct int_list, list_link);
			free(element);
		}
		return head_list;
	}
}

struct node * delete_at_end(struct node *head_list)
{
        struct node *element_node, *next_element_node;
        struct int_list *element;

        if(head_list->next == NULL)
        {
                return head_list;
        }
        else
        {
		element_node = head_list->next;
                next_element_node = element_node->next;
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

        if(position == 0)
        {
                printf("Invalid position. Positions should begin from 1.\n");
                return head_list;
        }

        if(head_list->next == NULL)
        {
        	printf("Can't delete an empty linked list \n");
		return head_list;
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

size_t size(struct node *head_list)
{
	size_t size = 0;
	struct node *element_node;

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


