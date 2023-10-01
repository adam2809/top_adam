#ifndef __TA_SAFE_QUEUE_H
#define __TA_SAFE_QUEUE_H

#include <threads.h>
#include "ta_queue.h"

void* ta_queue_safe_append(ta_queue* queue, void* val , mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty);
void* ta_queue_safe_pop(ta_queue* queue, mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty);

#endif