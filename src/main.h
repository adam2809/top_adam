#include <threads.h>
#include "ta_queue.h"

typedef struct ta_synch{
	ta_queue* cpu_info_queue;
	ta_queue* print_buffer;

	mtx_t cpu_info_queue_mtx;
	mtx_t print_buffer_mtx;
	cnd_t cpu_info_queue_full_cnd;
	cnd_t cpu_info_queue_empty_cnd;
	cnd_t print_buffer_modified;

	mtx_t watchdog_mtx;
	cnd_t watchdog_reader_cnd;
	cnd_t watchdog_analyzer_cnd;
	cnd_t watchdog_printer_cnd;

	_Atomic (int) finished;
} ta_synch;

int ta_synch_init(ta_synch* synch);
void ta_synch_destroy();
int reader_fun(void* arg);
int analyzer_fun(void* arg);
int printer_fun(void* arg);
int watchdog_fun(void* arg);
int watchdog_timeout(cnd_t* cnd, mtx_t* mtx);
void finish(int signum);