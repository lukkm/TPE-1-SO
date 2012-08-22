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
void* execute_dec (void*);
void init_processes(void);

process_t ml_process;

sem_t sem;

int main(void)
{
	char line[100];	
	init_processes();
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

