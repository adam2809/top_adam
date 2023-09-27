#include <threads.h>
#include <stdio.h>
#include "ta_queue.h"

#define PROC_STAT_FILE_PATH "/proc/stat"

int reader_thread(void* arg);
int analyzer_thread(void* arg);
int printer_thread(void* arg);

ta_queue* cpu_info_queue;
ta_queue* prev_cpu_info_queue;

FILE* proc_stat_file_ptr;

int reader_fun(void* arg){

}

int analyzer_fun(void* arg){

}

int printer_fun(void* arg){

}

int main(int argc, char **argv) {
	thrd_t reader_thrd;
	thrd_t analyzer_thrd;
	thrd_t printer_thrd;
	int thrd_create_ret;

	cpu_info_queue = ta_queue_new();
	prev_cpu_info_queue = ta_queue_new();

	if(!cpu_info_queue || !prev_cpu_info_queue){
		return 1;
	}

	proc_stat_file_ptr = fopen(PROC_STAT_FILE_PATH,"r");
	if(!proc_stat_file_ptr){
		return 1;
	}

	thrd_create_ret = thrd_create(&reader_thrd,reader_fun, 0);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_create_ret = thrd_create(&analyzer_thrd,analyzer_fun, 0);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_create_ret = thrd_create(&printer_thrd,printer_fun, 0);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_join(reader_thrd, 0);
	thrd_join(analyzer_thrd, 0);
	thrd_join(printer_thrd, 0);
	return 0;
}
