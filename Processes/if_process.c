#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void * execute_if (void *);
process_t if_process;

int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_if, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	if_process = calloc(1, sizeof(struct process));
	if_process->params = calloc(1, sizeof (struct ipc_params));
	if_process->params->file = "/tmp/if";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test if \n");
	ipc_open(if_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(if_process->params, line, 100) > 0){
			printf("El proceso if recibio: %s\n", line);
		}
		sleep(1);
	}
	return 0;
}


void * execute_if (void * structure_params)
{
	process_params_t par = (process_params_t) structure_params;

	printf("WTF \n");
	return NULL;
}
