#include "ta_safe_queue.h"

void* ta_queue_safe_append(ta_queue* queue, void* val , mtx_t* mtx, cnd_t* cnd_full, cnd_t* cnd_empty){
	void* ret;

	mtx_lock(mtx);

	if (ta_queue_is_full(queue))
	{
		cnd_wait(cnd_full,mtx);
	}

	ret = ta_queue_append(queue, val);
	if(ret){
		cnd_signal(cnd_empty);
	}

	mtx_unlock(mtx);
	return ret;
	
}
