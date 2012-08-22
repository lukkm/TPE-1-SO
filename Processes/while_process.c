#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

process_params_t pre_execute(status_t, int);
void call_next_process(status, ipc_params_t);
void init_processes(void);

process_t while_process;

int main(void)
{
	status c_program;
	graph_t mem;
	process_params_t thread_args;
	pthread_t thread_id;

	init_processes();
	ipc_open(while_process->params, O_RDONLY);
	while(1){
		if (ipc_receive(while_process->params, &c_program, sizeof(struct status)) > 0){ 
			if ( (long)(mem = (graph_t)shmat(c_program.g_header.fd, c_program.g_header.mem_adress, 0)) == -1 )
				fatal("shmat");
			printf("WHILE Process\n");
			mem->current = mem->current->true_node;
			if (mem->current != NULL)
				call_next_process(c_program, mem->current->instruction_process->instruction_type->params);
			shmdt(mem);
		}
		sleep(1);
	}
	
	return 0;
}
