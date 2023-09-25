#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "proc_stat.h"

#define PROC_STAT_SEVEN_FIELDS_PATH "test_data/proc_stat_seven_fields"
#define PROC_STAT_TEN_FIELDS_PATH "test_data/proc_stat_ten_fields"
#define PROC_STAT_MAX_CHARS 1000

void test_get_next_proc_stat_cpu_info_seven_fields();
void test_get_next_proc_stat_cpu_info_ten_fields();

int main(int argc, char **argv) {
	test_get_next_proc_stat_cpu_info_seven_fields();
	test_get_next_proc_stat_cpu_info_ten_fields();

	return 0;
}

void test_get_next_proc_stat_cpu_info_seven_fields(){
	proc_stat_cpu_info cpu0_info;
	proc_stat_cpu_info cpu1_info;

	FILE *fptr = fopen(PROC_STAT_SEVEN_FIELDS_PATH,"r");
	assert(fptr != 0);

	int ret_cpu0 = get_next_proc_stat_cpu_info(&cpu0_info,PROC_STAT_MAX_CHARS,fptr);
	int ret_cpu1 = get_next_proc_stat_cpu_info(&cpu1_info,PROC_STAT_MAX_CHARS,fptr);
	int ret_end = get_next_proc_stat_cpu_info(&cpu1_info,PROC_STAT_MAX_CHARS,fptr);

	fclose(fptr);

	assert(ret_cpu0 == 7);
	assert(cpu0_info.cpu_id     == 0);
	assert(cpu0_info.usertime   == 1132ULL);
	assert(cpu0_info.nicetime   == 34ULL);
	assert(cpu0_info.systemtime == 1441ULL);
	assert(cpu0_info.idletime   == 11311718ULL);
	assert(cpu0_info.iowait     == 3675ULL);
	assert(cpu0_info.irq        == 127ULL);
	assert(cpu0_info.softirq    == 438ULL);
	assert(cpu0_info.steal      == 0ULL);
	assert(cpu0_info.guest      == 0ULL);
	assert(cpu0_info.guest_nice == 0ULL);

	assert(ret_cpu1 == 7);
	assert(cpu1_info.cpu_id     == 1);
	assert(cpu1_info.usertime   == 1123ULL);
	assert(cpu1_info.nicetime   == 0ULL);
	assert(cpu1_info.systemtime == 849ULL);
	assert(cpu1_info.idletime   == 11313845ULL);
	assert(cpu1_info.iowait     == 2614ULL);
	assert(cpu1_info.irq        == 0ULL);
	assert(cpu1_info.softirq    == 18ULL);
	assert(cpu1_info.steal      == 0ULL);
	assert(cpu1_info.guest      == 0ULL);
	assert(cpu1_info.guest_nice == 0ULL);

	assert(ret_end == 0);
}

void test_get_next_proc_stat_cpu_info_ten_fields(){
	proc_stat_cpu_info cpu0_info;
	proc_stat_cpu_info cpu1_info;

	FILE *fptr = fopen(PROC_STAT_TEN_FIELDS_PATH,"r");
	assert(fptr != 0);

	int ret_cpu0 = get_next_proc_stat_cpu_info(&cpu0_info,PROC_STAT_MAX_CHARS,fptr);
	int ret_cpu1 = get_next_proc_stat_cpu_info(&cpu1_info,PROC_STAT_MAX_CHARS,fptr);
	int ret_end = get_next_proc_stat_cpu_info(&cpu1_info,PROC_STAT_MAX_CHARS,fptr);

	fclose(fptr);

	assert(ret_cpu0 == 10);
	assert(cpu0_info.cpu_id     == 0);
	assert(cpu0_info.usertime   == 1025113ULL);
	assert(cpu0_info.nicetime   == 173ULL);
	assert(cpu0_info.systemtime == 437442ULL);
	assert(cpu0_info.idletime   == 16917446ULL);
	assert(cpu0_info.iowait     == 10813ULL);
	assert(cpu0_info.irq        == 0ULL);
	assert(cpu0_info.softirq    == 29024ULL);
	assert(cpu0_info.steal      == 123ULL);
	assert(cpu0_info.guest      == 0ULL);
	assert(cpu0_info.guest_nice == 0ULL);

	assert(ret_cpu1 == 10);
	assert(cpu1_info.cpu_id     == 1);
	assert(cpu1_info.usertime   == 1069186ULL);
	assert(cpu1_info.nicetime   == 67ULL);
	assert(cpu1_info.systemtime == 454101ULL);
	assert(cpu1_info.idletime   == 5773811ULL);
	assert(cpu1_info.iowait     == 3886ULL);
	assert(cpu1_info.irq        == 0ULL);
	assert(cpu1_info.softirq    == 12644ULL);
	assert(cpu1_info.steal      == 0ULL);
	assert(cpu1_info.guest      == 88ULL);
	assert(cpu1_info.guest_nice == 23ULL);

	assert(ret_end == 0);
}
