#include <pthread.h>
#include <stdio.h>
#include "../structs.h"
#include "../defs.h"

int pre_execute(process_params_t);
void * execute_if (void *);


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
	process_params_t empty;	
	pre_execute(empty);
	printf("Test IF \n");
	return 0;
}


void * execute_if (void * structure_params)
{
	process_params_t par = (process_params_t) structure_params;

	printf("WTF \n");
	return NULL;
}
