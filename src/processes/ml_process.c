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

void* execute_ml (void*);
void init_processes(void);

process_t ml_process;

int main(void)
{
	status c_program;
	
	init_processes();
	
	ipc_open(ml_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(ml_process->params, &c_program, sizeof(struct status)) > 0)
			run_process(&c_program, &execute_ml);
	return 0;
}


void* execute_ml (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if ( par->c_status->cursor - par->param + 1 < 0)
	  par->c_status->cursor = 0;
	else
          par->c_status->cursor -= par->param;
	
	true_step(par);
	return NULL;
}

