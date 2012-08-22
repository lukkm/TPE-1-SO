#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "../structs.h"
#include "../defs.h"
#include "../IPCS/ipcs.h"

process_params_t pre_execute(status_t, int);
void call_next_process(status, ipc_params_t);
void* execute_endwhile (void*);
void init_processes(void);

process_t endwhile_process;

sem_t sem;

int main(void)
{
	status c_program;
	graph_t mem;
	process_params_t thread_args;
	pthread_t thread_id;

	init_processes();
	sem_init(&sem,0,0);
	ipc_open(endwhile_process->params, O_RDONLY);
	while(1){
		if (ipc_receive(endwhile_process->params, &c_program, sizeof(struct status)) > 0){ 
			if ( (mem = (graph_t)shmat(c_program.g_header.fd, c_program.g_header.mem_adress, 0)) == -1 )
				fatal("shmat");
			printf("ENDWHILE Process\n");
			mem->current = mem->current->true_node;
			if (current != NULL)
				call_next_process(c_program, mem->current->instruction_process->instruction_type->params);
			shmdt(mem);
		}
		sleep(1);
	}
	
	return 0;
}
