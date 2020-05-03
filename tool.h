#define _GNU_SOURCE
#include <sys/types.h>
#include <stdbool.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/wait.h>

#define UNIT_T() { for (volatile unsigned long i = 0 ; i < 1000000UL ; i++); }

struct process {
	char name[32];
	int ready_time;
	int exec_time;
	pid_t pid;
	int request_time;
};

int assign_cpu(int pid, int core);
int proc_setscheduler(int pid, int SCHED_var);
int proc_exec(struct process proc);