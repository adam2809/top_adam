#ifndef __PROC_STAT_H
#define __PROC_STAT_H

#include <stdio.h>

typedef struct proc_stat_cpu_info proc_stat_cpu_info;
struct proc_stat_cpu_info {
	int cpu_id;

	unsigned long long int usertime;
	unsigned long long int nicetime;
	unsigned long long int systemtime;
	unsigned long long int idletime;
	unsigned long long int iowait;
	unsigned long long int irq;
	unsigned long long int softirq;
	unsigned long long int steal;
	unsigned long long int guest;
	unsigned long long int guest_nice;
};

int init_proc_stat_cpu_info(proc_stat_cpu_info *const cpu_info);

// reads one line from proc stat and puts results in the proc_stat_cpu_info struct
int get_next_proc_stat_cpu_info(proc_stat_cpu_info *const cpu_info, size_t size, FILE* stream);

// takes the proc stat struct and analyzes it to get cpu percentage
double analyze_proc_stat_cpu_info(proc_stat_cpu_info const*const cpu_stats);

#endif
