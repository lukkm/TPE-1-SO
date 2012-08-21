#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_cz (void*);
void init_processes(void);

process_t cz_process;

int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	//pthread_create(&thread_id, NULL, &execute_cz, &thread_args);
	return 0;

}



int main(void)
{
	char line[100];	
	init_processes();
	printf("Test cz \n");
	ipc_open(cz_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(cz_process->params, line, 100) > 0){
			printf("El proceso cz recibio: %s\n", line);
		}
		sleep(1);
	}
	
	return 0;
}


void* execute_cz (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if (!par->c_status.mem[par->c_status.cursor])
	    par->c_status.flag = TRUE;
	else
            par->c_status.flag = FALSE;
		
	return NULL;
}











