#ifndef __PROC_STAT_H
#define __PROC_STAT_H

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

#endif
