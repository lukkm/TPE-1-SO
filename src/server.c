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


#include "../include/structs.h"
#include "../include/utils/parser.h"
#include "../include/defs.h"
#include "../include/data_structures/graph.h"
#include "../include/data_structures/stack.h"
#include "../include/ipcs/ipcs.h"

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
extern ipc_params_t client_params;

extern process_t * process_list[CANT_INSTRUCTIONS];
extern char * process_name_list[CANT_INSTRUCTIONS];

void init(void);
void init_processes(void);
void fatal(char *s);
graph_t create_sh_graph(graph_t, int, int*, shared_graph_header_t);
int get_graph_size(graph_t);

int
main(void) 
{
	stack_t c_stack;
	graph_t c_graph;

	char * read_string;
	int aux_size;
	int memkey, i;
	process_t process_type;
	status client_program;
	
	init_processes();
	init();
	
	client_header_t header = calloc(1, sizeof(struct client_header));
	
	while(1)
	{
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0)
		{
			
			read_string = calloc(1, header->program_size);
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0)
				sleep(1);
				
			printf("Recibi programa del cliente: %d\n", header->client_id);
			
			c_stack = parse_file(read_string);

			c_graph = build_graph(c_stack);

			if (c_graph != NULL)
			{
				create_sh_graph(c_graph, get_graph_size(c_graph), 
								&memkey, &client_program.g_header);
				client_program.cursor = 0;
				client_program.flag = FALSE;
				for(i = 0; i < MEM_SIZE; i++){
					client_program.mem[i] = 0;
				}
				process_type = c_graph->first->instruction_process->instruction_type;
				ipc_send(process_type->params, &client_program, sizeof(struct status));
			}else{
				printf("Entrada incorrecta\n");
			}
		}
		if (ipc_receive(server_receive_params, &client_program, sizeof(struct status))){
			ipc_open(client_params, O_WRONLY);
			ipc_send(client_params, &client_program, sizeof(struct status));
			ipc_close(client_params);
		}
		sleep(1);
	}
		

	return 0;
}

void init(){
	int i, j, k, pid;
	int string_length;
	int to_exec_length;
	char * to_exec;
	char * ct_bin = "bin/";
	
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	
	ipc_create(server_receive_params);
	ipc_open(server_receive_params, O_RDONLY|O_NONBLOCK);
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		
		ipc_create((*process_list[i])->params);
		
		switch(pid = fork()){
			case -1:
				perror("Fork exception");
				exit(1);
				break;
			case 0: /* Hijo */
				(*(process_list[i]))->pid = getpid();
				to_exec_length = strlen(process_name_list[i]) + BIN_LENGTH + 1;
				to_exec = calloc(1, to_exec_length);
				strcpy(to_exec, ct_bin);
				strcat(to_exec, process_name_list[i]);
				to_exec[to_exec_length - 1] = 0;
				execvp(to_exec, NULL);		
				
				/* No deberia llegar aca */
				perror("Process error");
				exit(1);
				break;
		}
		ipc_open((*(process_list[i]))->params, O_WRONLY);
		
	}

}
