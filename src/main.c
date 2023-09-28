#include <threads.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ta_queue.h"
#include "proc_stat.h"

#define PROC_STAT_FILE_PATH "/proc/stat"
#define PROC_STAT_MAX_LINE_LEN 1000

#define MAX_CPU_INFO_QUEUE_LEN 1000

int reader_thread(void* arg);
int analyzer_thread(void* arg);
int printer_thread(void* arg);

int is_cpu_info_unanalyzed(void* elem);

ta_queue* cpu_info_queue;
ta_queue* prev_cpu_info_queue;

FILE* proc_stat_file_ptr;

int reader_fun(void* arg){
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
			printf("-------------------\n");
			continue;
		}
		ta_queue_append(cpu_info_queue, cpu_info_ptr);
		printf("cpu%d %llu\n", cpu_info_ptr->cpu_id, cpu_info_ptr->usertime);
	}
}

int analyzer_fun(void* arg){
	proc_stat_cpu_info* next;
	proc_stat_cpu_info* prev;

	while (1)
	{
		next = ta_queue_find(cpu_info_queue,is_cpu_info_unanalyzed);
		if(!next){
			continue;
		}

		prev = ta_queue_elem(prev_cpu_info_queue,next->cpu_id);
		if (!prev)
		{
			proc_stat_cpu_info* prev_new = new_proc_stat_cpu_info();
			memcpy(prev_new,next,sizeof(proc_stat_cpu_info));
			ta_queue_append(prev_cpu_info_queue,prev_new);
		}
		next->cpu_usage_percent = analyze_proc_stat_cpu_info(next, prev);
	}
}

int is_cpu_info_unanalyzed(void* elem){
	proc_stat_cpu_info* cpu_info = elem;
	return cpu_info->cpu_usage_percent < 0;
}

int printer_fun(void* arg){
	proc_stat_cpu_info* cpu_info_queue_head;
	proc_stat_cpu_info* cpu_info_queue_next;

	while (1)
	{
		do{
			cpu_info_queue_head = ta_queue_peek(cpu_info_queue);
		}while (is_cpu_info_unanalyzed((void*) prev_cpu_info_queue));

		cpu_info_queue_head = ta_queue_pop(cpu_info_queue);
		printf("cpu%d %.2f\n", cpu_info_queue_head->cpu_id, cpu_info_queue_head->cpu_usage_percent);
		cpu_info_queue_next = ta_queue_peek(cpu_info_queue);
		if(cpu_info_queue_next->cpu_id < cpu_info_queue_head->cpu_id){
			printf("-----------------------");
		}
	}
}

int main(int argc, char **argv) {
	thrd_t reader_thrd;
	thrd_t analyzer_thrd;
	thrd_t printer_thrd;
	int thrd_create_ret;

	cpu_info_queue = ta_queue_new(MAX_CPU_INFO_QUEUE_LEN);
	prev_cpu_info_queue = ta_queue_new(-1);

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
