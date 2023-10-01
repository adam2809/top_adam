#include <threads.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "proc_stat.h"
#include "main.h"
#include "ta_logger.h"

#define PROC_STAT_FILE_PATH "/proc/stat"
#define PROC_STAT_MAX_LINE_LEN 1000

#define MAX_CPU_INFO_QUEUE_LEN 1000

#define WATCHDOG_TIMEOUT_SEC 2

ta_synch synch;

int reader_fun(void* arg){
	ta_synch* synch = arg;

	FILE* proc_stat_file_ptr = 0;

	while(!synch->finished){
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
		ta_queue_append(synch->cpu_info_queue, cpu_info_ptr);
	}
}

int analyzer_fun(void* arg){
	ta_synch* synch = arg;

	proc_stat_cpu_info* next;
	proc_stat_cpu_info* prev;

	while (!synch->finished)
	{
		next = ta_queue_find(synch->cpu_info_queue,is_cpu_info_unanalyzed);
		if(!next){
			continue;
		}

		prev = ta_queue_elem(synch->prev_cpu_info_queue,next->cpu_id);
		if (!prev)
		{
			proc_stat_cpu_info* prev_new = new_proc_stat_cpu_info();
			memcpy(prev_new,next,sizeof(proc_stat_cpu_info));
			ta_queue_append(synch->prev_cpu_info_queue,prev_new);
		}
		next->cpu_usage_percent = analyze_proc_stat_cpu_info(next, prev);
	}
}

int is_cpu_info_unanalyzed(void* elem){
	proc_stat_cpu_info* cpu_info = elem;
	return cpu_info->cpu_usage_percent < 0;
}

int printer_fun(void* arg){
	ta_synch* synch = arg;
	proc_stat_cpu_info* cpu_info_queue_head;
	proc_stat_cpu_info* cpu_info_queue_next;

	while (!synch->finished)
	{
		do{
			cpu_info_queue_head = ta_queue_peek(synch->cpu_info_queue);
		}while (is_cpu_info_unanalyzed((void*) synch->prev_cpu_info_queue));

		cpu_info_queue_head = ta_queue_pop(synch->cpu_info_queue);
		printf("cpu%d %.2f\n", cpu_info_queue_head->cpu_id, cpu_info_queue_head->cpu_usage_percent);
		cpu_info_queue_next = ta_queue_peek(synch->cpu_info_queue);
		if(cpu_info_queue_next->cpu_id < cpu_info_queue_head->cpu_id){
			printf("-----------------------");
		}
	}
}

int watchdog_fun(void* arg){
	int ret;
	ta_synch* synch = arg;

	while(!synch->finished){
		ret = watchdog_timeout(&synch->watchdog_reader_cnd,&synch->watchdog_mtx);
		if (ret == thrd_timedout)
		{
			puts("Reader not reported to watchdog");
		}

		ret = watchdog_timeout(&synch->watchdog_analyzer_cnd,&synch->watchdog_mtx);
		if (ret == thrd_timedout)
		{
			puts("Analyzer not reported to watchdog");
		}

		ret = watchdog_timeout(&synch->watchdog_printer_cnd,&synch->watchdog_mtx);
		if (ret == thrd_timedout)
		{
			puts("Printer not reported to watchdog");
		}
	}
	return 0;
}

int watchdog_timeout(cnd_t* cnd, mtx_t* mtx){
	struct timespec now;
	timespec_get(&now, TIME_UTC);
	now.tv_sec += WATCHDOG_TIMEOUT_SEC;
	return cnd_timedwait(cnd, mtx, &now);
}

int ta_synch_init(ta_synch* synch){
	memset(synch,0,sizeof(synch));

	synch->cpu_info_queue = ta_queue_new(MAX_CPU_INFO_QUEUE_LEN);
	synch->prev_cpu_info_queue = ta_queue_new(-1);

	if(!synch->cpu_info_queue || !synch->prev_cpu_info_queue){
		return 1;
	}

	int ret = 0;

	ret |= mtx_init(&synch->cpu_info_queue_mtx, mtx_plain);
	ret |= cnd_init(&synch->cpu_info_queue_full_cnd);
	ret |= cnd_init(&synch->cpu_info_queue_full_cnd);
	ret |= cnd_init(&synch->cpu_info_queue_head_analyzed_cnd);

	ret |= mtx_init(&synch->watchdog_mtx, mtx_plain);
	ret |= cnd_init(&synch->watchdog_reader_cnd);
	ret |= cnd_init(&synch->watchdog_analyzer_cnd);
	ret |= cnd_init(&synch->watchdog_printer_cnd);

	if(ret){
		return 1;
	}

	synch->finished = 0;

	return 0;
}

void ta_synch_destroy(ta_synch* synch){
	ta_queue_destroy(synch->cpu_info_queue);
	ta_queue_destroy(synch->prev_cpu_info_queue);

	mtx_destroy(&synch->cpu_info_queue_mtx);
	cnd_destroy(&synch->cpu_info_queue_full_cnd);
	cnd_destroy(&synch->cpu_info_queue_full_cnd);
	cnd_destroy(&synch->cpu_info_queue_head_analyzed_cnd);

	mtx_destroy(&synch->watchdog_mtx);
	cnd_destroy(&synch->watchdog_reader_cnd);
	cnd_destroy(&synch->watchdog_analyzer_cnd);
	cnd_destroy(&synch->watchdog_printer_cnd);
}

void finish(int signum){
	synch.finished = 1;
	ta_logger_stop();
}

int main(int argc, char **argv) {
	thrd_t reader_thrd;
	thrd_t analyzer_thrd;
	thrd_t printer_thrd;
	thrd_t watchdog_thrd;
	int thrd_create_ret;

#ifdef TA_LOGGER_ENABLE
	thrd_t logger_thrd;
	int logger_init_ret;
	logger_init_ret = ta_logger_init();
	if (logger_init_ret == 0)
	{
		return 1;
	}
	
	thrd_create_ret = thrd_create(&logger_thrd, ta_logger_core, 0);
	if(thrd_create_ret != thrd_success){
		return 1;
	}
#endif

	ta_log("---------------------------------");
	ta_log("Starting top_adam");

	if(ta_synch_init(&synch) != 0){
		return 1;
	}

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = finish;
	if(sigaction(SIGTERM, &action, 0)){
		ta_log("Failed to initialize capturing SIGTERM");
		return 1;
	}

	thrd_create_ret = thrd_create(&reader_thrd,reader_fun, &synch);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_create_ret = thrd_create(&analyzer_thrd,analyzer_fun, &synch);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_create_ret = thrd_create(&watchdog_thrd,watchdog_fun, &synch);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_create_ret = thrd_create(&printer_thrd,printer_fun, &synch);
	if(thrd_create_ret != thrd_success){
		return 1;
	}

	thrd_join(reader_thrd, 0);
	thrd_join(analyzer_thrd, 0);
	thrd_join(printer_thrd, 0);
	thrd_join(watchdog_thrd, 0);
#ifdef TA_LOGGER_ENABLE
	thrd_join(logger_thrd, 0);
#endif

	ta_synch_destroy(&synch);
	ta_logger_destroy();

	return 0;
}
