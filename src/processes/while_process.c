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

process_params_t pre_execute(status_t, int);
void call_next_process(status, ipc_params_t);
void init_processes(void);

process_t while_process;
ipc_params_t server_receive_params;

int main(void)
{
	status c_program;
	graph_t mem;
	process_params_t thread_args;
	pthread_t thread_id;

	init_processes();
	ipc_open(while_process->params, O_RDONLY);
	printf("WHILE IPCS: %d\n", while_process->params->unique_mq_id);
	while(1){
		if (ipc_receive(while_process->params, &c_program, sizeof(struct status)) > 0){ 
			if ( (long)(mem = (graph_t)shmat(c_program.g_header.fd, c_program.g_header.mem_adress, 0)) == -1 )
				fatal("shmat");
			printf("WHILE Process\n");
			if (!mem->current->cond_executed){
				mem->current->cond_executed = 1;
				mem->current = mem->current->conditional_expr;
			}else{
				mem->current->cond_executed = 0;
				if (c_program.flag){
					mem->current = mem->current->true_node;
				}else{
					mem->current = mem->current->false_node;
				}
			}
			if (mem->current != NULL)
				call_next_process(c_program, mem->current->instruction_process->instruction_type->params);
			else{
				shmctl(c_program.g_header.fd, IPC_RMID, 0);
				call_next_process(c_program, server_receive_params);
			}
			shmdt(mem);
		}
		sleep(1);
	}
	
	return 0;
}
