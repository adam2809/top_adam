#include "ta_queue.h"
#include <stdlib.h>

ta_node *ta_queue_new_node();

ta_node *ta_queue_new_node(){
	return calloc(1,sizeof(ta_node));
}

ta_queue *ta_queue_new()
{
	ta_queue *new = malloc(sizeof(ta_queue));
	new->start = 0;
	new->end = 0;
	return new;
}

void *ta_queue_append(ta_queue *queue, void *val)
{
	ta_node *old_end = queue->end;
	ta_node *new_node = ta_queue_new_node();
	if (!new_node)
	{
		return 0;
	}
	new_node->val = val;

	if (old_end == 0)
	{
		queue->start = new_node;
	}
	else
	{
		old_end->next = new_node;
	}
	queue->end = new_node;

	return new_node->val;
}
void *ta_queue_pop(ta_queue *queue)
{
	if (ta_queue_is_empty(queue))
	{
		return 0;
	}

	ta_node *old_start = queue->start;
	void *ret = old_start->val;

	if (old_start == queue->end)
	{
		queue->start = 0;
		queue->end = 0;
	}
	else
	{
		queue->start = old_start->next;
	}
	free(old_start);

	return ret;
}
void ta_queue_destroy(ta_queue *queue)
{
	ta_node *node;
	while (1)
	{
		node = ta_queue_pop(queue);
		if (node)
		{
			free(node->val);
		}else{
			break;
		}
		
	}

	free(queue);
}

void* ta_queue_elem(ta_queue *queue, int n){
	ta_node* elem = queue->start;

	while(n != 0 && elem != 0){
		elem = elem->next;
		n--;
	}

	if (!elem)
	{
		return 0;
	}else{
		return elem->val;
	}
	
}

int ta_queue_is_empty(ta_queue *queue){
	return queue->start == 0;
}

void* ta_queue_find(ta_queue *queue, int (*cmp)(void*)){
	if(ta_queue_is_empty(queue)){
		return 0;
	}

	ta_node* elem = queue->start;
	while(elem){
		if(cmp(elem)){
			return elem->val;
		}
		elem = elem->next;
	}

	return 0;
}