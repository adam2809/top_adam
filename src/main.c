#include "ta_queue.h"

int reader_thread(void* arg);
int analyzer_thread(void* arg);
int printer_thread(void* arg);

ta_queue* cpu_info_queue;
ta_queue* prev_cpu_info_queue;

int main(int argc, char **argv) {
	cpu_info_queue = ta_queue_new();
	prev_cpu_info_queue = ta_queue_new();

	if(!cpu_info_queue || !prev_cpu_info_queue){
		return 1;
	}
	return 0;
}
