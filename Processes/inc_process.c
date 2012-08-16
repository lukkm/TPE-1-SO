#include <pthread.h>
#include <stdio.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_inc (void*);


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
	process_params_t empty;	
	pre_execute(empty);
	printf("Test INC \n");

	return 0;
}


void* execute_inc (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	par->c_status.mem[par->c_status.cursor] += par->param;
	return NULL;
}

