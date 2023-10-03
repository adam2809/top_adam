#include "ta_logger.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <threads.h>
#include <stdlib.h>
#include "ta_safe_queue.h"


#define MAX_QUEUE_SIZE 50

FILE* log_file_ptr = 0;
ta_queue* log_queue;
mtx_t log_queue_mtx;
cnd_t log_queue_full_cnd;
cnd_t log_queue_empty_cnd;
_Atomic (int) logger_finished = 0;

ta_logger_msg* ta_logger_msg_new(char* msg){
	ta_logger_msg* new = malloc(sizeof(ta_logger_msg));
	if(new == 0){
		return 0;
	}

	new->time = time(0);
	size_t msg_strlen = strlen(msg);
	if(msg_strlen >= sizeof(new->msg)){
		memcpy(new->msg,msg,sizeof(new->msg)-1);
		new->msg[sizeof(new->msg)-1] = 0;
	}else{
		strcpy(new->msg, msg);
	}

	return new;
}

int ta_logger_init(){
	#ifdef TA_LOGGER_ENABLE
	log_file_ptr = fopen(TA_LOGGER_FILE_PATH,"a");
	if(log_file_ptr == 0){
		return 0;
	}
	
	log_queue = ta_queue_new(MAX_QUEUE_SIZE);
	if(!log_queue){
		return 0; 
	}
	mtx_init(&log_queue_mtx,mtx_plain);
	cnd_init(&log_queue_full_cnd);
	cnd_init(&log_queue_empty_cnd);
	return 1;
	#else
	return 0;
	#endif
}

int ta_logger_core(void* arg){
	#ifdef TA_LOGGER_ENABLE
	ta_logger_msg* log_msg;
	while(!logger_finished){
		if(!log_file_ptr){
			return 1;
		}

		log_msg = ta_queue_safe_pop(
			log_queue,
			&log_queue_mtx,
			&log_queue_full_cnd,
			&log_queue_empty_cnd
		);

		fprintf(log_file_ptr,"[%ju] %s\n", (uintmax_t) log_msg->time, log_msg->msg);
		fflush(log_file_ptr);
		free(log_msg);
	}
	return 0;
	#else
	return 1;
	#endif
}

void ta_log(char* msg){
	#ifdef TA_LOGGER_ENABLE
	if (logger_finished)
	{
		return;
	}
	
	ta_logger_msg* log_msg = ta_logger_msg_new(msg);
	if(!log_msg){
		return;
	}

	void* ret = ta_queue_safe_append(
		log_queue,
		log_msg,
		&log_queue_mtx,
		&log_queue_full_cnd,
		&log_queue_empty_cnd
	);
	#endif
}

void ta_logger_stop(){
	logger_finished = 1;
}

void ta_logger_destroy(){
	ta_queue_destroy(log_queue);

	mtx_destroy(&log_queue_mtx);
	cnd_destroy(&log_queue_full_cnd);
	cnd_destroy(&log_queue_empty_cnd);

	fclose(log_file_ptr);
}