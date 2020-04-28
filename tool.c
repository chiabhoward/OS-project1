#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <time.h>
#include "tool.h"

int assign_cpu(int pid, int core)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
		
	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
		perror("sched_setaffinity");
		exit(1);
	}

	return 0;
}

int proc_setscheduler(int pid, int SCHED_var)
{
	struct sched_param param;
	param.sched_priority = 0;

	if( sched_setscheduler(pid, SCHED_var, &param) < 0) {
		perror("sched_setscheduler");
		return -1;
	}

	return 0;
}

int proc_exec(struct process p)
{
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork error");
		exit(1);
	}
	if (pid == 0) { /*child */
		int id = getpid();

		long start_s, finish_s;
		
		start_s = syscall(333);

		for (int t = 0; t < p.exec_time; t++) {
			UNIT_T();
		}
		
		finish_s = syscall(333);
		syscall(334, id, start_s, finish_s);
		exit(0);
	}
	/* parent */ 
	assign_cpu(pid, 1);
	return pid;
}