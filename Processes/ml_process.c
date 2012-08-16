#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_ml (void*);
process_t ml_process;


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_ml, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	ml_process = calloc(1, sizeof(struct process));
	ml_process->params = calloc(1, sizeof (struct ipc_params));
	ml_process->params->file = "/tmp/ml";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test ml \n");
	ipc_open(ml_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(ml_process->params, line, 100) > 0){
			printf("El proceso ml recibio: %s\n", line);
		}
		sleep(1);
	}
	return 0;
}


void* execute_ml (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if ( par->c_status.cursor - par->param + 1 < 0)
	  par->c_status.cursor = 0;
	else
          par->c_status.cursor -= par->param;
		
	return NULL;
}

