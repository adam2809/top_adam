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
void ta_queue_destroy(ta_queue *queue);

#endif