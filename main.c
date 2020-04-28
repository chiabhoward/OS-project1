#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "scheduler.h"

int main(int argc, char const *argv[])
{
	char scheduler_policy[16];
	assert(scanf("%s", scheduler_policy) == 1);

	int process_num;
	assert(scanf("%d", &process_num) == 1);

	struct process *proc;
	proc = (struct process *)malloc(process_num * sizeof(struct process));

	for (int n = 0; n < process_num; n++) {
		scanf("%s%d%d", proc[n].name, &proc[n].ready_time, &proc[n].exec_time);
		proc[n].pid = -1;
	}

	int policy;
	switch(scheduler_policy[0]) {
		case 'F':
			policy = FIFO;
			break;
		case 'R':
			policy = RR;
			break;
		case 'S':
			policy = SJF;
			break;
		case 'P':
			policy = PSJF;
			break;
	}
	scheduling(proc, process_num, policy);
	exit(0);	
}