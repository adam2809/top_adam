#include "ta_safe_queue.h"

void* ta_queue_safe_append(ta_queue* queue, void* val , mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty){
	void* ret;

	if(mtx_lock(mtx) != thrd_success) return 0;

	if (ta_queue_is_full(queue))
	{
		if(cnd_wait(cnd_full,mtx) != thrd_success) return 0;
	}

	ret = ta_queue_append(queue, val);
	if(ret){
		if(cnd_signal(cnd_empty) != thrd_success) return 0;
	}

	if(mtx_unlock(mtx) != thrd_success) return 0;
	return ret;
	
}

void* ta_queue_safe_pop(ta_queue* queue, mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty){
	void* ret;

	if(mtx_lock(mtx) != thrd_success) return 0;

	if(ta_queue_is_empty(queue)){
		if(cnd_wait(cnd_empty,mtx) != thrd_success) return 0;
	}
	ret = ta_queue_pop(queue);
	if(cnd_signal(cnd_full) != thrd_success) return 0;

	if(mtx_unlock(mtx) != thrd_success) return 0;

	return ret;
}

ta_node* ta_queue_safe_append_nullable(ta_queue* queue, void* val , mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty){
	ta_node* ret;

	mtx_lock(mtx);

	if (ta_queue_is_full(queue))
	{
		cnd_wait(cnd_full,mtx);
	}

	ret = ta_queue_append_nullable(queue, val);
	if(ret){
		cnd_signal(cnd_empty);
	}

	mtx_unlock(mtx);
	return ret;
	
}

ta_node* ta_queue_safe_pop_nullable(ta_queue* queue, mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty){
	ta_node* ret;

	mtx_lock(mtx);

	if(ta_queue_is_empty(queue)){
		cnd_wait(cnd_empty,mtx);
	}
	ret = ta_queue_pop_nullable(queue);
	cnd_signal(cnd_full);

	mtx_unlock(mtx);

	return ret;
}