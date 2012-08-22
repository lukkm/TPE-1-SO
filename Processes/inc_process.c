#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include "../structs.h"
#include "../defs.h"
#include "../IPCS/ipcs.h"

int pre_execute(process_params_t);
void* execute_inc (void*);
void init_processes(void);

process_t inc_process;


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_inc, &thread_args);
	return 0;

}



int main(void)
{
	
	char line[100];	
	shared_graph_header header;
	graph_t mem;
	init_processes();
	printf("Test inc\n");
	ipc_open(inc_process->params, O_RDONLY);
	while(ipc_receive(inc_process->params, &header, sizeof(struct shared_graph_header)) == 0)
		sleep(1);
	while(1){
		if (ipc_receive(inc_process->params, line, 100) > 0){ 
			if ( (mem = (graph_t)shmat(header.fd, header.mem_adress, 0)) == -1 ){
				fatal("shmat");
			}
		}
		sleep(1);
	}
	
	return 0;
}


void* execute_inc (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	par->c_status.mem[par->c_status.cursor] += par->param;
	return NULL;
}

