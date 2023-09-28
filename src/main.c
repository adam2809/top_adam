#include <threads.h>
#include <stdlib.h>
#include <stdio.h>
#include "ta_queue.h"
#include "proc_stat.h"

#define PROC_STAT_FILE_PATH "/proc/stat"
#define PROC_STAT_MAX_LINE_LEN 1000

int reader_thread(void* arg);
int analyzer_thread(void* arg);
int printer_thread(void* arg);

ta_queue* cpu_info_queue;
ta_queue* prev_cpu_info_queue;

FILE* proc_stat_file_ptr;

int reader_fun(void* arg){
	FILE* proc_stat_file_ptr = 0;

	while(1){
		if(!proc_stat_file_ptr){
			proc_stat_file_ptr = fopen(PROC_STAT_FILE_PATH,"r");
		}
		if(!proc_stat_file_ptr){
			return 1;
		}

		proc_stat_cpu_info* cpu_info_ptr = new_proc_stat_cpu_info();
		if(cpu_info_ptr == 0){
			continue;
		}

		int res = get_next_proc_stat_cpu_info(cpu_info_ptr,PROC_STAT_MAX_LINE_LEN,proc_stat_file_ptr);
		if(res == -1){
			free(cpu_info_ptr);
			continue;
		}
		if(res == 0){
			free(cpu_info_ptr);
			fclose(proc_stat_file_ptr);
			proc_stat_file_ptr = 0;
			continue;
		}
		ta_queue_append(cpu_info_queue, cpu_info_ptr);
	}
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
