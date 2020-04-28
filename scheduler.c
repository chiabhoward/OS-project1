#define _GnprocU_SOURCE
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

int cmp( const void * a, const void *b){
	return ( (struct process*) a ) -> ready_time - ( (struct process*) b ) -> ready_time;
}


static int t_last;

static int ntime;

static int current;

static int finish_cnt;

int next_process( struct process *proc, int nproc, int policy) {
	if ( (policy == FIFO || policy == SJF) && current != -1)
		return current;

	else if( policy == FIFO ) {
		for(int i = 0 ; i < nproc ; i++) 
			if( proc[i].pid != -1 )
				return i;

		return -1; 
	}

	else if( policy == RR ) {
		if( current == -1 ) {
			for(int i = 0; i < nproc; i++)
				if( proc[i].pid != -1 && proc[i].exec_time > 0) 
					return i; 
		}
		else if ( (ntime - t_last) % 500 == 0 ) {
			for(int i = current + 1 ; i < nproc+current ; i++) {
				if( proc[(i%nproc)].pid != -1 )
					return (i%nproc); 
			}
		}
		return current;
	}

	else{
		int min_idx = -1;
		for( int i = 0 ; i < nproc ; i++){
			if( proc[i].pid == -1 || proc[i].exec_time == 0 )
				continue;
			
			if( min_idx == -1 || proc[i].exec_time < proc[min_idx].exec_time )
				min_idx = i; 
		}

		if( min_idx == -1 ) return -1;

		if( current != -1 && proc[min_idx].exec_time == proc[current].exec_time ) return current;
		else return min_idx;
	}
	
}

int scheduling(struct process *proc, int nproc, int policy)
{
	qsort(proc, nproc, sizeof(struct process), cmp);

	assign_cpu(getpid(), 0);

	proc_setscheduler(getpid(), SCHED_OTHER);

	t_last = -1;
	ntime = 0;
	current = -1;
	finish_cnt = 0;

	while (1) {
		if( current != -1 && proc[current].exec_time == 0 ){
			waitpid(proc[current].pid, NULL, 0);

			printf("%s %d\n", proc[current].name, proc[current].pid);
			fflush(stdout);
			proc[current].pid = -1;

			current = -1;
			finish_cnt++;

			if( finish_cnt == nproc) break;
		}
		
		for( int i = 0 ; i < nproc ; i++){
			if( proc[i].exec_time == 0 || proc[i].pid != -1 )
				continue;
			
			if( proc[i].ready_time == ntime ) {
				proc[i].pid = proc_exec(proc[i]);
				proc_setscheduler(proc[i].pid, SCHED_IDLE);
			}
		}

		int next = next_process(proc, nproc, policy);
		if (next != -1) {
			if (current != next) {
				proc_setscheduler(proc[next].pid, SCHED_OTHER); 
				proc_setscheduler(proc[current].pid, SCHED_IDLE);
				current = next;
				t_last = ntime;
			}
		}

		UNIT_T();
		if( current != -1 )
			proc[current].exec_time--;
		ntime++;
	}



	return 0;	
}