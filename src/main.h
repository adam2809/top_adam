#include <threads.h>
#include "ta_queue.h"

typedef struct ta_synch{
	ta_queue* cpu_info_queue;
	ta_queue* prev_cpu_info_queue;

	mtx_t cpu_info_queue_mtx;
	cnd_t cpu_info_queue_full_cnd;
	cnd_t cpu_info_queue_all_analyzed_cnd;
	cnd_t cpu_info_queue_head_analyzed_cnd;

	mtx_t watchdog_mtx;
	cnd_t watchdog_reader_cnd;
	cnd_t watchdog_analyzer_cnd;
	cnd_t watchdog_printer_cnd;
} ta_synch;

int ta_synch_init(ta_synch* synch);
int reader_fun(void* arg);
int analyzer_fun(void* arg);
int printer_fun(void* arg);
int watchdog_fun(void* arg);
int is_cpu_info_unanalyzed(void* elem);
int watchdog_timeout(cnd_t* cnd, mtx_t* mtx);