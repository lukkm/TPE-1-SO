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

void* execute_mr (void*);

process_t mr_process;

int main(void)
{
	status c_program;
	
	init_processes();
	
	signal(SIGINT, end_process);
	
	ipc_open(mr_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(mr_process->params, &c_program, sizeof(struct status)) > 0)
			run_process(&c_program, &execute_mr);
	return 0;
}


void* execute_mr (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if ( par->c_status->cursor + par->param - 1 > 1000)
	  par->c_status->cursor = 999;
	else
          par->c_status->cursor += par->param;

	true_step(par);
	return NULL;
}

