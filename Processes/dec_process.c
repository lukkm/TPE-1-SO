#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_dec (void*);

process_t dec_process;


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_dec, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	dec_process = calloc(1, sizeof(struct process));
	dec_process->params = calloc(1, sizeof (struct ipc_params));
	dec_process->params->file = "/tmp/dec";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test dec \n");
	ipc_open(dec_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(dec_process->params, line, 100) > 0){
			printf("El proceso dec recibio: %s\n", line);
		}
		sleep(1);
	}
	return 0;
}


void* execute_dec (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if (par->c_status.mem[par->c_status.cursor] >= par->param)
	 par->c_status.mem[par->c_status.cursor] -= par->param;
	else
	 par->c_status.mem[par->c_status.cursor] = 0;

	return NULL;
}

