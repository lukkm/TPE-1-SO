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

process_t if_process;
ipc_params_t server_receive_params;

sem_t sem;

int main(void)
{
	status c_program;
	graph_t mem;
	process_params_t thread_args;
	pthread_t thread_id;

	init_processes();
	sem_init(&sem,0,0);
	ipc_open(if_process->params, O_RDONLY);
	while(1){
		if (ipc_receive(if_process->params, &c_program, sizeof(struct status)) > 0){ 
			if ( (long)(mem = (graph_t)shmat(c_program.g_header.fd, c_program.g_header.mem_adress, 0)) == -1 )
				fatal("shmat");
			printf("If process\n");			
			if (!mem->current->cond_executed){
				mem->current->cond_executed = 1;
				mem->current = mem->current->conditional_expr;
			}else{
				mem->current->cond_executed = 0;
				if (c_program.flag){
					printf("voy true\n");
					mem->current = mem->current->true_node;
				}else{
					printf("voy false\n");
					mem->current = mem->current->false_node;
					printf("%p\n", mem->current);
				}
			}
			if (mem->current != NULL){
				call_next_process(c_program, mem->current->instruction_process->instruction_type->params);
			}
			else {
				call_next_process(c_program, server_receive_params);
				shmctl(c_program.g_header.fd, IPC_RMID, 0);
			}
			shmdt(mem);
		}
	}
	
	return 0;
}
