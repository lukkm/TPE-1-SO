#include <pthread.h>
#include <stdio.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_dec (void*);


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	pthread_create(&thread_id, NULL, &execute_dec, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	process_params_t empty;	
	pre_execute(empty);
	printf("Test DEC \n");
	return 0;
}


void* execute_dec (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if (par->c_status->mem[par->c_status->cursor] >= par->param)
	 par->c_status->mem[par->c_status->cursor] -= par->param;
	else
	 par->c_status->mem[par->c_status->cursor] = 0;

	return NULL;
}

