#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

#include "../include/structs.h"
#include "../include/utils/parser.h"
#include "../include/defs.h"
#include "../include/data_structures/graph.h"
#include "../include/data_structures/stack.h"
#include "../include/utils/process_utils.h"
#include "../include/utils/shmem_graph.h"
#include "../include/ipcs/ipcs.h"
#include "../include/utils/ipcs_protocol.h"

extern process_t inc_process;
extern process_t dec_process;
extern process_t mr_process;
extern process_t ml_process;
extern process_t cz_process;
extern process_t if_process;
extern process_t endif_process;
extern process_t while_process;
extern process_t endwhile_process;

extern ipc_params_t server_receive_params;
extern ipc_params_t server_params;

extern process_t * process_list[CANT_INSTRUCTIONS];
extern char * process_name_list[CANT_INSTRUCTIONS];

sem_t sem;

void init(void);
void fatal(char *s);
void * run_program(void * program_name);
void server_close();
void * run_server_receive(void * params);
void answer_error_to_client(int err_code, int client_id);

struct thread_status{
	char * file;
	int client_id;
};
typedef struct thread_status * thread_status_t;

int
main(void) 
{
	char * read_string;
	int aux_size;
	
	pthread_t thread_id, receive_thread_id;
	thread_status_t thread_info;
	
	signal(SIGINT, server_close);
	
	init_processes(TRUE);
	init();
	
	client_header_t header = calloc(1, sizeof(struct client_header));
	
	pthread_create(&receive_thread_id, NULL, &run_server_receive, NULL);
	
	printf("Running server...\n");
	
	while(1)
	{
		server_params->msg_type = PRE_HEADER;
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0)
		{
			read_string = calloc(1, header->program_size);
			server_params->msg_type = PROGRAM_STRING;
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0)
				sleep(1);
			printf("Received program from client: %d, program name: %s\n", 
										header->client_id, read_string);
			thread_info = calloc(1, sizeof(struct thread_status));
			thread_info->file = read_string;
			thread_info->client_id = header->client_id;
			pthread_create(&thread_id, NULL, &run_program, thread_info);
			pthread_join(thread_id, NULL);
			free(read_string);
			free(thread_info);
		}
		sleep(1);
	}
	return 0;
}

void * run_program(void * program_stat)
{
	graph_t c_graph;
	int memkey, i;
	status client_program;
	process_t process_type;
	mstack_t instruction_stack;
	
	thread_status_t thread_info = (thread_status_t)program_stat;

	instruction_stack = parse_file((char*)thread_info->file);	

	if (instruction_stack != NULL){
	
		c_graph = build_graph(instruction_stack);

		if (c_graph != NULL)
		{
			create_sh_graph(c_graph, get_graph_size(c_graph), 
							&memkey, &client_program.g_header);
			client_program.cursor = 0;
			client_program.flag = FALSE;
			client_program.client_id = thread_info->client_id;
			
			for(i = 0; i < MEM_SIZE; i++){
				client_program.mem[i] = 0;
			}
			
			process_type = c_graph->first->instruction_process->instruction_type;
			client_program.mtype = process_type->params->unique_mq_id;
			free_graph(c_graph);
			free_stack(instruction_stack);
			printf("Sending first instruction...\n");
			ipc_send(process_type->params, &client_program, sizeof(struct status));
		}else{
			answer_error_to_client(UNABLE_TO_BUILD_GRAPH, thread_info->client_id); 
		}
	}else{
		
		answer_error_to_client(errno, thread_info->client_id);
		errno = 0;
	}
	pthread_exit(NULL);
}

void init()
{
	int i, j, k, pid, aux_semid;
	int string_length;
	int to_exec_length;
	char * to_exec;
	int ct_length;
	char * ct_path = get_ipc_path();
	
	ct_length = strlen(ct_path);
	
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	
	aux_semid = init_sem();
	
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		
		switch(pid = fork()){
			case -1:
				perror("Fork exception");
				exit(1);
				break;
			case 0: /* Hijo */
				(*(process_list[i]))->pid = getpid();
				to_exec_length = strlen(process_name_list[i]) + ct_length + 1;
				to_exec = calloc(1, to_exec_length);
				strcpy(to_exec, ct_path);
				strcat(to_exec, process_name_list[i]);
				to_exec[to_exec_length - 1] = 0;
				execvp(to_exec, NULL);		
				/* No deberia llegar aca */
				perror("Process error");
				exit(1);
				break;
		}
		(*(process_list[i]))->params->unique_id = pid;
		(*process_list[i])->params->socklistener = TRUE;
		ipc_create((*process_list[i])->params);	
		ipc_open((*(process_list[i]))->params, O_WRONLY);
	}
	
	free(ct_path);

}


void server_close(){

	int i;

	printf("Closing server...\n");

	for(i = 0; i < CANT_INSTRUCTIONS; i++)
		ipc_destroy((*(process_list[i]))->params);

	semctl(server_params->semid, 293845, IPC_RMID);

	ipc_destroy(server_params);
	ipc_destroy(server_receive_params);
	
	free_processes();

	printf("Server closed.\n");
	exit(0);
}


void * run_server_receive(void * params){
	
	ipc_params_t client_params;
	status client_final;
	
	ipc_create(server_receive_params);
	ipc_open(server_receive_params, O_RDONLY|O_NONBLOCK);
	
	while(1){
		if(ipc_receive(server_receive_params, &client_final, sizeof(struct status))){
			client_params = get_params_from_pid(client_final.client_id, PROGRAM_STATUS, sizeof(struct status), server_params->semid);			
			client_params->socklistener = TRUE;
			printf("Answering to client: %d\n", client_final.client_id);
			ipc_open(client_params, O_WRONLY);
			ipc_send(client_params, &client_final, sizeof(struct status));
			ipc_close(client_params);
			free(client_params->file);
			free(client_params);
		}
		sleep(1);
	}
}

void answer_error_to_client(int err_code, int client_id){
	
	ipc_params_t client_params;
	status client_program;
	
	client_params = get_params_from_pid(client_id, PROGRAM_STATUS, sizeof(struct status), server_params->semid);			
	client_params->socklistener = TRUE;
	client_program.flag = err_code;
	printf("Answering to client: %d\n", client_id);
	ipc_open(client_params, O_WRONLY);
	ipc_send(client_params, &client_program, sizeof(struct status));
	ipc_close(client_params);
	free(client_params->file);
	free(client_params);
	
}
