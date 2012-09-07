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

void* execute_cz (void*);

process_t cz_process;
int i = 0;

int main(void)
{
	status c_program;
	
	init_processes();
	
	signal(SIGINT, end_process);
	
	ipc_open(cz_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(cz_process->params, &c_program, sizeof(struct status)) > 0){
			printf("Recibi algo wacho, soy cz, CZ!!!\n");
			run_process(&c_program, &execute_cz);
		}
	return 0;
}


void* execute_cz (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if (!par->c_status->mem[par->c_status->cursor])
	    par->c_status->flag = TRUE;
	else
            par->c_status->flag = FALSE;
	
	true_step(par);
	return NULL;
}











