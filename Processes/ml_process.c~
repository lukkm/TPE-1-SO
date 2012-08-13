#include <pthread.h>
#include <stdio.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void* execute_ml (void*);


int pre_execute(process_params_t params)
{
  
	pthread_t thread_id;	
	process_params_t thread_args = params;
	pthread_create(&thread_id, NULL, &execute_ml, &thread_args);
	return 0;

}



int main(void)
{
	/* Useless */
	process_params_t empty;	
	pre_execute(empty);
	printf("Test ML \n");
	return 0;
}


void* execute_ml (void* structure_params)
{
	process_params_t par = (process_params_t) structure_params;
	
	if ( par->c_status->cursor - par->param + 1 < 0)
	  par->c_status->cursor = 0;
	else
          par->c_status->cursor -= par->param;
		
	return NULL;
}

