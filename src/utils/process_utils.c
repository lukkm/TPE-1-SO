#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"
#include "../../include/utils/process_utils.h"

ipc_params_t server_receive_params;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * call_function(void * v_params);

typedef struct {
	status_t v_program;
	void * (* v_execute_func) (void *);
} call_params;

process_params_t pre_execute(status_t program, graph_t mem)
{
  	process_params_t ret = (process_params_t)calloc(1, sizeof(struct process_params));
	ret->param = mem->current->instruction_process->param;
	ret->c_status = program;
	ret->sh_graph = mem;
	return ret;
}

void call_next_process(status c_status, ipc_params_t params){
	ipc_open(params, O_WRONLY);
	ipc_send(params, &c_status, sizeof(struct status));
	ipc_close(params);
	return;
}

void take_next_step(process_params_t par)
{
	graph_t sh_graph = par->sh_graph;	
	pthread_mutex_lock(&mutex);

	if (sh_graph->current != NULL) {
		call_next_process(*par->c_status, sh_graph->current->instruction_process->instruction_type->params);
	}else{
		call_next_process(*par->c_status, server_receive_params);
		shmctl(par->c_status->g_header.fd, IPC_RMID, 0);
	}
	shmdt(sh_graph);

	pthread_mutex_unlock(&mutex);
}

void false_step(process_params_t par)
{	
	par->sh_graph->current = par->sh_graph->current->false_node;
	take_next_step(par);
}

void conditional_step(process_params_t par)
{	
	par->sh_graph->current = par->sh_graph->current->conditional_expr;
	take_next_step(par);
}

void true_step(process_params_t par)
{
	par->sh_graph->current = par->sh_graph->current->true_node;
	take_next_step(par);
}

void run_process(status_t c_status, void * (* execute_func) (void *))
{	
	call_params v_params;
	
	pthread_t thread_id;
	
	v_params.v_program = c_status;
	v_params.v_execute_func = execute_func;
	
	//pthread_create(&thread_id, NULL, &call_function, &v_params);	
	call_function(&v_params);
}

void * call_function(void * v_params)
{
	process_params_t thread_args;
	pthread_t thread_id;
	
	graph_t mem;
	
	status_t c_status = ((call_params *)v_params)->v_program;
	void * (*exec_func) (void *) = 
							((call_params *)v_params)->v_execute_func;
	
	pthread_mutex_lock(&mutex);

	if ( (long)(mem = (graph_t)shmat(c_status->g_header.fd, 
							c_status->g_header.mem_adress, 0)) == -1 )
		fatal("shmat");

	thread_args = pre_execute(c_status, mem);

	pthread_mutex_unlock(&mutex);
	pthread_create(&thread_id, NULL, exec_func, thread_args);
	//func(thread_args);
}

ipc_params_t get_params_from_pid(int pid, int type, int shm_size)
{
	ipc_params_t params = calloc(1, sizeof(struct ipc_params));
	int aux = pid, cont = 0;
	char * ct_tmp = "/tmp/";
	
	params->unique_id = pid;
	params->msg_type = type;
	params->shm_segment_size = shm_size;
	
	while(aux > 0)
	{
		aux /= 10;
		cont++;
	}
	cont+=5;
	
	params->file = calloc(cont+TMP_LENGTH+1, sizeof(char));
	strcpy(params->file, ct_tmp);
	
	params->file[cont--] = 0;
	
	while(pid > 0)
	{
		params->file[cont--] = pid % 10;
		pid /= 10;
	}
	
	return params;
}
