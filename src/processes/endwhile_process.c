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

void* execute_endwhile (void*);
void init_processes(void);

process_t endwhile_process;

int main(void)
{
	status c_program;
	
	init_processes();
	ipc_open(endwhile_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(endwhile_process->params, &c_program, sizeof(struct status)) > 0)
			run_process(&c_program, &execute_endwhile);
	return 0;
}

void * execute_endwhile(void * structure_params){
	process_params_t par = (process_params_t) structure_params;
	true_step(par);
	return NULL;
}
