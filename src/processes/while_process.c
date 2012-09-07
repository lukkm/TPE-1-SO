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

void* execute_while (void*);

int i = 0;

process_t while_process;
ipc_params_t server_receive_params;

int main(void)
{
	status c_program;
	
	init_processes();
	
	signal(SIGINT, end_process);
	
	ipc_open(while_process->params, O_RDONLY);
	
	while(1)
		if (ipc_receive(while_process->params, &c_program, sizeof(struct status)) > 0){
			printf("Recibi algo wacho, soy while, WHILE!!!\n");
			run_process(&c_program, &execute_while);
		}
	return 0;
}

void * execute_while(void * structure_params){
	
		process_params_t par = (process_params_t) structure_params;
		graph_t mem = par->sh_graph;
		//if (par->c_status->cursor == 559){
		//	printf("%d\n", par->c_status->cursor);
		//}
		//if (par->c_status->cursor == ){
		
		if (!mem->current->cond_executed){
			mem->current->cond_executed = 1;
			conditional_step(par);
		}else{
			mem->current->cond_executed = 0;
			
			if (par->c_status->flag)
				true_step(par);
			else
				false_step(par);
		}
		
}
