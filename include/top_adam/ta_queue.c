#include "ta_queue.h"
#include <stdlib.h>

ta_node *ta_queue_new_node();

ta_node *ta_queue_new_node(){
	return calloc(1,sizeof(ta_node));
}

ta_queue *ta_queue_new(int max_len)
{
	ta_queue *new = malloc(sizeof(ta_queue));
	new->start = 0;
	new->end = 0;
	new->len = 0;
	new->max_len = max_len;
	return new;
}

void *ta_queue_append(ta_queue *queue, void *val)
{
	ta_node* ret = ta_queue_append_nullable(queue,val);
	if (ret){
		return ret->val;
	}

	return 0;
}
void *ta_queue_pop(ta_queue *queue)
{
	ta_node* node = ta_queue_pop_nullable(queue);
	void* ret = node->val;

	if(ret){
		free(node);
		return ret;
	}

	return 0;
}

ta_node* ta_queue_append_nullable(ta_queue* queue,void* val){
	if (queue->max_len !=-1 && ta_queue_is_full(queue))
	{
		return 0;
	}
	
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

	queue->len++;

	return new_node;
}

ta_node* ta_queue_pop_nullable(ta_queue *queue){
	if (ta_queue_is_empty(queue))
	{
		return 0;
	}

	ta_node *old_start = queue->start;

	if (old_start == queue->end)
	{
		queue->start = 0;
		queue->end = 0;
	}
	else
	{
		queue->start = old_start->next;
	}

	queue->len--;
	return old_start;
}

void* ta_queue_peek(ta_queue *queue){
	return ta_queue_elem(queue,0);
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
	if(n >= queue->len){
		return 0;
	}

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

int ta_queue_is_full(ta_queue *queue){
	return queue->len >= queue->max_len;
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

int ta_queue_is_head(ta_queue *queue, void* elem){
	return elem == ta_queue_peek(queue);
}