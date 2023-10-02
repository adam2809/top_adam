#ifndef __TA_QUEUE_H
#define __TA_QUEUE_H

typedef struct ta_node ta_node;

struct ta_node
{
	void *val;
	ta_node *next;
};

typedef struct ta_queue
{
	ta_node *start;
	ta_node *end;
	int len;
	int max_len;
} ta_queue;

ta_queue *ta_queue_new(int max_len);
void* ta_queue_append(ta_queue* queue,void* val);
void *ta_queue_pop(ta_queue *queue);
ta_node* ta_queue_append_nullable(ta_queue* queue,void* val);
ta_node* ta_queue_pop_nullable(ta_queue *queue);
void ta_queue_destroy(ta_queue *queue);
void* ta_queue_elem(ta_queue *queue, int n);
void* ta_queue_peek(ta_queue *queue);
void* ta_queue_find(ta_queue *queue, int (*cmp)(void*));
int ta_queue_is_empty(ta_queue *queue);
int ta_queue_is_full(ta_queue *queue);
int ta_queue_is_head(ta_queue *queue, void* elem);

#endif
