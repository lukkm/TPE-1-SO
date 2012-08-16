#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_while (void*);
process_t while_process;


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_while, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	while_process = calloc(1, sizeof(struct process));
	while_process->params = calloc(1, sizeof (struct ipc_params));
	while_process->params->file = "/tmp/while";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test WHILE \n");
	ipc_open(while_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(while_process->params, line, 100) > 0){
			printf("El proceso WHILE recibio: %s\n", line);
		}
		sleep(1);
	}
	return 0;
}


void* execute_while (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	printf("WTF \n");
	return NULL;
}
