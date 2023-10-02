#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "proc_stat.h"

#define PROC_STAT_SEVEN_FIELDS_PATH "test_data/proc_stat_seven_fields"
#define PROC_STAT_TEN_FIELDS_PATH "test_data/proc_stat_ten_fields"
#define PROC_STAT_MAX_CHARS 1000
#define PROC_STAT_LINES_TO_TEST 4

void test_get_next_proc_stat_cpu_info_seven_fields();
void test_get_next_proc_stat_cpu_info_ten_fields();

int main(int argc, char **argv) {
	test_get_next_proc_stat_cpu_info_seven_fields();
	test_get_next_proc_stat_cpu_info_ten_fields();

	return 0;
}

void test_get_next_proc_stat_cpu_info_seven_fields(){
	proc_stat_cpu_info cpu_info[PROC_STAT_LINES_TO_TEST];
	int ret[PROC_STAT_LINES_TO_TEST];

	FILE *fptr = fopen(PROC_STAT_SEVEN_FIELDS_PATH,"r");
	assert(fptr != 0);

	for (int i = 0; i < PROC_STAT_LINES_TO_TEST; i++)
	{
		init_proc_stat_cpu_info(&cpu_info[i]);
		ret[i] = get_next_proc_stat_cpu_info(&cpu_info[i], PROC_STAT_MAX_CHARS, fptr);
	}

	fclose(fptr);

	assert(ret[0] == 7);
	assert(cpu_info[0].cpu_id     == -1);
	assert(cpu_info[0].usertime   == 2255LL);
	assert(cpu_info[0].nicetime   == 34LL);
	assert(cpu_info[0].systemtime == 2290LL);
	assert(cpu_info[0].idletime   == 22625563LL);
	assert(cpu_info[0].iowait     == 6290LL);
	assert(cpu_info[0].irq        == 127LL);
	assert(cpu_info[0].softirq    == 456LL);
	assert(cpu_info[0].steal      == 0ULL);
	assert(cpu_info[0].guest      == 0ULL);
	assert(cpu_info[0].guest_nice == 0ULL);

	assert(ret[1] == 8);
	assert(cpu_info[1].cpu_id     == 0);
	assert(cpu_info[1].usertime   == 1132ULL);
	assert(cpu_info[1].nicetime   == 34ULL);
	assert(cpu_info[1].systemtime == 1441ULL);
	assert(cpu_info[1].idletime   == 11311718ULL);
	assert(cpu_info[1].iowait     == 3675ULL);
	assert(cpu_info[1].irq        == 127ULL);
	assert(cpu_info[1].softirq    == 438ULL);
	assert(cpu_info[1].steal      == 0ULL);
	assert(cpu_info[1].guest      == 0ULL);
	assert(cpu_info[1].guest_nice == 0ULL);

	assert(ret[2] == 8);
	assert(cpu_info[2].cpu_id     == 1);
	assert(cpu_info[2].usertime   == 1123ULL);
	assert(cpu_info[2].nicetime   == 0ULL);
	assert(cpu_info[2].systemtime == 849ULL);
	assert(cpu_info[2].idletime   == 11313845ULL);
	assert(cpu_info[2].iowait     == 2614ULL);
	assert(cpu_info[2].irq        == 0ULL);
	assert(cpu_info[2].softirq    == 18ULL);
	assert(cpu_info[2].steal      == 0ULL);
	assert(cpu_info[2].guest      == 0ULL);
	assert(cpu_info[2].guest_nice == 0ULL);

	assert(ret[3] == 0);
}

void test_get_next_proc_stat_cpu_info_ten_fields(){
	proc_stat_cpu_info cpu_info[PROC_STAT_LINES_TO_TEST];
	int ret[PROC_STAT_LINES_TO_TEST];

	FILE *fptr = fopen(PROC_STAT_TEN_FIELDS_PATH,"r");
	assert(fptr != 0);

	for (int i = 0; i < PROC_STAT_LINES_TO_TEST; i++)
	{
		ret[i] = get_next_proc_stat_cpu_info(&cpu_info[i], PROC_STAT_MAX_CHARS, fptr);
	}

	fclose(fptr);

	assert(ret[0] == 10);
	assert(cpu_info[0].cpu_id     == -1);
	assert(cpu_info[0].usertime   == 12290514ULL);
	assert(cpu_info[0].nicetime   == 1372ULL);
	assert(cpu_info[0].systemtime == 4699910ULL);
	assert(cpu_info[0].idletime   == 80829807ULL);
	assert(cpu_info[0].iowait     == 57099ULL);
	assert(cpu_info[0].irq        == 0ULL);
	assert(cpu_info[0].softirq    == 219181ULL);
	assert(cpu_info[0].steal      == 0ULL);
	assert(cpu_info[0].guest      == 0ULL);
	assert(cpu_info[0].guest_nice == 0ULL);

	assert(ret[1] == 11);
	assert(cpu_info[1].cpu_id     == 0);
	assert(cpu_info[1].usertime   == 1025113ULL);
	assert(cpu_info[1].nicetime   == 173ULL);
	assert(cpu_info[1].systemtime == 437442ULL);
	assert(cpu_info[1].idletime   == 16917446ULL);
	assert(cpu_info[1].iowait     == 10813ULL);
	assert(cpu_info[1].irq        == 0ULL);
	assert(cpu_info[1].softirq    == 29024ULL);
	assert(cpu_info[1].steal      == 123ULL);
	assert(cpu_info[1].guest      == 0ULL);
	assert(cpu_info[1].guest_nice == 0ULL);

	assert(ret[1] == 11);
	assert(cpu_info[2].cpu_id     == 1);
	assert(cpu_info[2].usertime   == 1069186ULL);
	assert(cpu_info[2].nicetime   == 67ULL);
	assert(cpu_info[2].systemtime == 454101ULL);
	assert(cpu_info[2].idletime   == 5773811ULL);
	assert(cpu_info[2].iowait     == 3886ULL);
	assert(cpu_info[2].irq        == 0ULL);
	assert(cpu_info[2].softirq    == 12644ULL);
	assert(cpu_info[2].steal      == 0ULL);
	assert(cpu_info[2].guest      == 88ULL);
	assert(cpu_info[2].guest_nice == 23ULL);

	assert(ret[3] == 0);
}
