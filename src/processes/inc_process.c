#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"
#include "../../include/utils/process_utils.h"
#include "../../include/utils/ipcs_protocol.h"

void* execute_inc (void*);

process_t inc_process;

int i = 0;

int main(void)
{
	status c_program;
	
	init_processes(FALSE);
	
	ipc_open(inc_process->params, O_RDONLY);
	
	signal(SIGINT, end_process);
	
	while(1)
		if (ipc_receive(inc_process->params, &c_program, sizeof(struct status)) > 0)
			run_process(&c_program, &execute_inc);
		
	return 0;
}


void* execute_inc (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	par->c_status->mem[par->c_status->cursor] += par->param;
	true_step(par);
}
