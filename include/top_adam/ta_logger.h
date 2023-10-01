#ifndef __TA_LOGGER_H
#define __TA_LOGGER_H

#include <time.h>

#define MAX_MSG_LEN 150

typedef struct ta_logger_msg{
	time_t time;
	char msg[MAX_MSG_LEN];
} ta_logger_msg;
#undef MAX_MSG_LEN

ta_logger_msg* ta_logger_msg_new();
int ta_logger_init();
int ta_logger_core(void* arg);
void ta_log(char* msg);
void ta_logger_destroy();
void ta_logger_stop();


#endif