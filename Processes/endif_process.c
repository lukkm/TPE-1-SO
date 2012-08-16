#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_endif (void*);
process_t endif_process;

int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_endif, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	endif_process = calloc(1, sizeof(struct process));
	endif_process->params = calloc(1, sizeof (struct ipc_params));
	endif_process->params->file = "/tmp/endif";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test endif \n");
	ipc_open(endif_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(endif_process->params, line, 100) > 0){
			printf("El proceso endif recibio: %s\n", line);
		}
		sleep(1);
	}
	return 0;
}


void* execute_endif (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	printf("WTF \n");
	return NULL;
}
