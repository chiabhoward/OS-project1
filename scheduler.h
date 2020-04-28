#include "tool.h"

#define FIFO	1
#define RR		2
#define SJF		3
#define PSJF	4

int scheduling(struct process *proc, int nproc, int policy);
