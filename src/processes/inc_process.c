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
void* execute_inc (void*);
void init_processes(void);

process_t inc_process;
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
	ipc_open(inc_process->params, O_RDONLY);
	while(1){
		if (ipc_receive(inc_process->params, &c_program, sizeof(struct status)) > 0){ 
			if ( (long)(mem = (graph_t)shmat(c_program.g_header.fd, c_program.g_header.mem_adress, 0)) == -1 )
				fatal("shmat");
			thread_args = pre_execute(&c_program, mem->current->instruction_process->param);
			printf("Inc process\n");			
			printf("Estado ANTES: %d\n", c_program.mem[c_program.cursor]); 
			pthread_create(&thread_id, NULL, &execute_inc, thread_args);			
			sem_wait(&sem);
			printf("Estado DESPUES: %d\n", c_program.mem[c_program.cursor]); 
			mem->current = mem->current->true_node;
			if (mem->current != NULL)
				call_next_process(c_program, mem->current->instruction_process->instruction_type->params);
			else{
				call_next_process(c_program, server_receive_params);
				shmctl(c_program.g_header.fd, IPC_RMID, 0);
			}
			shmdt(mem);
		}
	}
	
	return 0;
}


void* execute_inc (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	par->c_status->mem[par->c_status->cursor] += par->param;
	sem_post(&sem);	
	return NULL;
}
