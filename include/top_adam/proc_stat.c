#include "proc_stat.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define PROC_STAT_CPU_PREFIX "cpu"
#define PROC_STAT_CPU_PREFIX_STRLEN (sizeof(PROC_STAT_CPU_PREFIX)-1)

char* read_proc_stat_line(size_t size, char s[restrict size], FILE* restrict stream);
int read_proc_stat_prefix(char s[PROC_STAT_CPU_PREFIX_STRLEN], FILE* restrict stream);

int init_proc_stat_cpu_info(proc_stat_cpu_info *const cpu_info){
	memset(cpu_info,0,sizeof(proc_stat_cpu_info));
}

int get_next_proc_stat_cpu_info(proc_stat_cpu_info *const cpu_info, size_t size, FILE* stream){
	int ret;
	int next_cpu_id = -1;
	char proc_stat_line[size];
	char* proc_stat_line_pos;

	char proc_stat_prefix[PROC_STAT_CPU_PREFIX_STRLEN];

	while(1){
		if(read_proc_stat_prefix(proc_stat_prefix,stream) == EOF){
			return -1;
		}

		if(memcmp(proc_stat_prefix, PROC_STAT_CPU_PREFIX, PROC_STAT_CPU_PREFIX_STRLEN)){
			return 0;
		}

		if(!read_proc_stat_line(size,proc_stat_line,stream)){
			return -1;
		}

		if(isdigit(proc_stat_line[0])){
			break;
		}
	}

	ret = sscanf(
		proc_stat_line,
		"%d %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
		&cpu_info->cpu_id,
		&cpu_info->usertime,
		&cpu_info->nicetime,
		&cpu_info->systemtime,
		&cpu_info->idletime,
		&cpu_info->iowait,
		&cpu_info->irq,
		&cpu_info->softirq,
		&cpu_info->steal,
		&cpu_info->guest,
		&cpu_info->guest_nice
	);

	return ret-1;
}

int read_proc_stat_prefix(char prefix[PROC_STAT_CPU_PREFIX_STRLEN], FILE* restrict stream){
	for(int i=0;i < PROC_STAT_CPU_PREFIX_STRLEN;i++){
		prefix[i] = fgetc(stream);
		if(prefix[i] == EOF){
			return EOF;
		}
	}
}

char* read_proc_stat_line(size_t size, char s[restrict size], FILE* restrict stream){
	s[0] = 0;

	char* ret = fgets(s, size, stream);
	if (ret){
		char* pos = strchr(s, '\n');

		if (pos){
			*pos = 0;
		}else{
			ret = 0;
		}
	}

	return ret;
}

double analyze_proc_stat_cpu_info(proc_stat_cpu_info* next,proc_stat_cpu_info* prev){
	unsigned long long prev_idle = prev->idletime + prev->iowait;
	unsigned long long idle = next->idletime + next->iowait;

	unsigned long long prev_non_idle = prev->usertime + prev->nicetime + prev->systemtime + prev->irq + prev->softirq + prev->steal;
	unsigned long long non_idle = next->usertime + next->nicetime + next->systemtime + next->irq + next->softirq + next->steal;

	unsigned long long prev_total = prev_idle + prev_non_idle;
	unsigned long long total = idle + non_idle;

	unsigned long long totald = total - prev_total;
	unsigned long long idled = idle - prev_idle;

	return (totald - idled)/totald;
}
