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


#include "structs.h"
#include "parser.h"
#include "defs.h"
#include "Data_Structures/graph.h"
#include "Data_Structures/stack.h"
#include "IPCS/ipcs.h"

extern process_t inc_process;
extern process_t dec_process;
extern process_t mr_process;
extern process_t ml_process;
extern process_t cz_process;
extern process_t if_process;
extern process_t endif_process;
extern process_t while_process;
extern process_t endwhile_process;

extern process_t * process_list[CANT_INSTRUCTIONS];
extern char * process_name_list[CANT_INSTRUCTIONS];
extern ipc_params_t ipcs_com_info[CANT_INSTRUCTIONS * (CANT_INSTRUCTIONS - 1)];

void init(void);
void init_processes(void);
void fatal(char *s);
graph_t create_sh_graph(graph_t, int, int*, shared_graph_header_t);
int get_graph_size(graph_t);

int
main(void) 
{
	init_processes();
	init();
	stack_t c_stack;
	graph_t c_graph;

	char * read_string;
	int aux_size;
	int memkey;
	process_t process_type;
	status client_program;
	
	client_header_t header = calloc(1, sizeof(struct client_header));


	ipc_params_t server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = "/tmp/server";
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	while(1)
	{
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0)
		{
			
			read_string = calloc(1, header->program_size);
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0)
				sleep(1);

			c_stack = parse_file(read_string);

			c_graph = build_graph(c_stack);

			if (c_graph != NULL)
			{
				create_sh_graph(c_graph, get_graph_size(c_graph), 
								&memkey, &client_program.g_header);
				client_program.cursor = 0;
				client_program.flag = FALSE;
				process_type = c_graph->first->instruction_process->instruction_type;
				ipc_send(process_type->params, &client_program, sizeof(struct status));
			}else{
				printf("Entrada incorrecta\n");
			}
		}
		sleep(1);
	}
		

	return 0;
}

void init(){
	int i, j, k, pid;
	int string_length;
	int to_exec_length;
	char * ct_process = "Processes/";
	char * to_exec;
	
	int com_length;
	char * com_file;
	
	
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		
		ipc_create((*process_list[i])->params);
		
		for(j = 0; j < CANT_INSTRUCTIONS; j++){
			if (i != j){
				com_length = strlen((*process_list[i])->params->file) + strlen(process_name_list[j])+1;
				com_file = calloc(1, com_length);
				strcpy(com_file, (*process_list[i])->params->file);
				strcat(com_file, process_name_list[j]);
				com_file[com_length-1] = 0;
				ipc_params_t ipc_com = calloc(1, sizeof(struct ipc_params));
				ipc_com->file = com_file;
				ipcs_com_info[i*(CANT_INSTRUCTIONS) + j] = ipc_com;
				ipc_create(ipc_com);
				free(com_file);
			}
		}
		
		switch(pid = fork()){
			case -1:
				perror("Fork exception");
				exit(1);
				break;
			case 0: /* Hijo */
				(*(process_list[i]))->pid = getpid();
				to_exec_length = strlen(process_name_list[i]) + PROCESSES_LENGTH + 1;
				to_exec = calloc(1, to_exec_length);
				for (j = 0; j < PROCESSES_LENGTH; j++)
					to_exec[j] = ct_process[j];
				for (j = 0; j < to_exec_length - PROCESSES_LENGTH; j++)
					to_exec[j+PROCESSES_LENGTH] = process_name_list[i][j];
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
