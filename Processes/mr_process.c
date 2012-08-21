#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_mr (void*);
void init_processes(void);

process_t mr_process;


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_mr, &thread_args);
	return 0;

}



int main(void)
{
	char line[100];	
	init_processes();
	printf("Test mr \n");
	ipc_open(mr_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(mr_process->params, line, 100) > 0){
			printf("El proceso mr recibio: %s\n", line);
		}
		sleep(1);
	}
	
	return 0;
}


void* execute_mr (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if ( par->c_status.cursor + par->param - 1 > 1000)
	  par->c_status.cursor = 999;
	else
          par->c_status.cursor += par->param;
		
	return NULL;
}

