#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"
#include "../../include/utils/process_utils.h"

void* execute_inc (void*);
void init_processes(void);

process_t inc_process;

int i = 0;

int main(void)
{
	status c_program;
	
	init_processes();
	
	ipc_open(inc_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(inc_process->params, &c_program, sizeof(struct status)) > 0) {
			if (i == 32750)
				printf("a\n");
			run_process(&c_program, &execute_inc);
		}
	return 0;
}


void* execute_inc (void* structure_params)
{
	printf("inc %d\n", i++);
	
	process_params_t par = (process_params_t) structure_params;
	par->c_status->mem[par->c_status->cursor] += par->param;
	true_step(par);
}
