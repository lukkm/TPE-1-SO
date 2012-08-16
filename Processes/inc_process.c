#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../structs.h"
#include "../defs.h"
#include "../IPCS/ipcs.h"

int pre_execute(process_params_t);
void* execute_inc (void*);

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
	/*EN BASE A IPC ARMO STRUCTURA AUX PARA EL THREAD*/

	/*Ejemplo	*/
	/*process_params_t ej = (process_params_t) calloc(1,sizeof(struct process_params))*/
	/*ej->c_status = (status_t) calloc(1,sizeof(struct status));
	ej->params = 5;
	ej->c_status->cursor = 30;
	ej->c_status->flag = FALSE;
	ej->c_status->id = 20;	
	/*Fin de Ejemplo*/
	
	/* Useless */
	inc_process = calloc(1, sizeof(struct process));
	inc_process->params = calloc(1, sizeof (struct ipc_params));
	inc_process->params->file = "/tmp/inc";
	
	process_params_t empty;
	char line[100];	
	pre_execute(empty);
	printf("Test INC \n");
	ipc_open(inc_process->params, O_RDONLY|O_NONBLOCK);
	while(1){
		if (ipc_receive(inc_process->params, line, 100) > 0){
			printf("El proceso INC recibio: %s\n", line);
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

