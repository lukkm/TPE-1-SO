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
void create_sh_graph(graph_t, int, int*, shared_graph_header_t);
int get_graph_size(graph_t);
node_t copy_graph(node_t, void *, int *);

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
	shared_graph_header g_header;
	
	client_header_t header = calloc(1, sizeof(struct client_header));


	ipc_params_t server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = "/tmp/server";
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	while(1){
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0){
			
			read_string = calloc(1, header->program_size);
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0){
				sleep(1);
			} 

			c_stack = parse_file(read_string);

			c_graph = build_graph(c_stack);

			if (c_graph != NULL){
				create_sh_graph(c_graph, get_graph_size(c_graph), 
								&memkey, &g_header);
				ipc_send(inc_process->params, &g_header, sizeof(struct shared_graph_header));
				ipc_send(inc_process->params, "holis", 5);
			}else{
				printf("Escribi bien, pelotudo\n");
			}
		}
		sleep(1);
	}
		

	return 0;
}

void create_sh_graph(graph_t c_graph, int size, int * memkey, shared_graph_header_t header)
{
	graph_t sh_graph;
	int cursor;
	void * mem;
	
	if ( (*memkey = shmget(221193, size, IPC_CREAT|0666)) == -1 )
		fatal("shmget");
	if ( (mem = shmat(*memkey, NULL, 0)) == -1 )
		fatal("shmat");
	header->fd = *memkey;
	header->mem_adress = mem; 
	header->size = size;
	if ((cursor = w_copy_graph(c_graph, mem)) == size)
		printf("BIEEEEN CAMPEON DEL MUNDO, BARRILETE COSMICO\n");
	else
		printf("cursor: %d\n", cursor);
	sh_graph = (graph_t)mem;
	//shmdt(mem);
	//shmctl(*memkey, IPC_RMID, 0);
	// CORRER PASANDOLE memid	
}

void * copy_node(void * sh_graph, node_t cur_node, int * cursor)
{
	node_t sh_node;
	int aux_size, start = *cursor;
	
	if (cur_node == NULL)
		return NULL;
	
	aux_size = sizeof(struct graph_node);
	sh_node = (node_t)memcpy(sh_graph + start, cur_node, aux_size) ;
	
	start += aux_size;
	aux_size = sizeof(struct instruction);
	sh_node->instruction_process = (instruction_t)memcpy
			(sh_graph + start, cur_node->instruction_process, aux_size);
	sh_node->instruction_process->expr = NULL;
	start += aux_size;
	aux_size = sizeof(struct process);
	sh_node->instruction_process->instruction_type = 
					(process_t)memcpy(sh_graph + start, 
					cur_node->instruction_process->instruction_type, 
					aux_size);
	start += aux_size;
	aux_size = sizeof(struct ipc_params);
	sh_node->instruction_process->instruction_type->params = 
		(ipc_params_t)memcpy(sh_graph + start, cur_node->
		instruction_process->instruction_type->params, aux_size);
	start += aux_size;
	aux_size = strlen(sh_node->instruction_process->instruction_type->
						params->file)+1;
	sh_node->instruction_process->instruction_type->params->file = 
	(char *)memcpy(sh_graph + start, cur_node->instruction_process->
					instruction_type->params->file, aux_size);
	start += aux_size;
	*cursor = start;
	sh_node->conditional_expr = copy_graph(cur_node->conditional_expr, 
											sh_graph, cursor);
	return sh_node;
}

int w_copy_graph(graph_t c_graph, void * sh_graph)
{
	int cursor = 0, aux_size;
	
	aux_size = sizeof(graph);
	
	sh_graph = memcpy(sh_graph, c_graph, aux_size);
	cursor += aux_size;
	
	if ((((graph_t)sh_graph)->first = 
				copy_graph(c_graph->first, sh_graph, &cursor)) == NULL)
		return -1;
	
	((graph_t)sh_graph)->current = ((graph_t)sh_graph)->first;
	
	return cursor;
}

node_t copy_graph(node_t c_node, void * sh_graph, 
				int * cursor)
{
	int aux_size, aux_type, flag = 0;
	node_t aux_sh_node, aux_c_node = c_node, sh_node;
	stack_t aux_stack = create_stack();
	
	if (c_node == NULL)
		return NULL;
	sh_node = sh_graph + *cursor;
	aux_sh_node = sh_node;
	aux_c_node = c_node;
	
	while (aux_c_node != NULL)
	{
		if ((aux_sh_node = copy_node(sh_graph, aux_c_node, 
									cursor)) == NULL)
			return NULL;
		if (flag)
		{
			if (!is_empty(aux_stack))
			{
				*((node_t*)pop(aux_stack)) = aux_sh_node;
				flag = 0;
			} else {
				return NULL;
			}
		}
		aux_type = 
				aux_c_node->instruction_process->instruction_type->type;
		if (aux_type == ENDIF || aux_type == ENDWHILE)
			flag = 1;
		if (aux_c_node->false_node != NULL)
			push(aux_stack, &aux_sh_node->false_node);
		else
			aux_sh_node->false_node = NULL;
		aux_sh_node->true_node = sh_graph + *cursor;
		aux_sh_node = aux_sh_node->true_node;
		aux_c_node = aux_c_node->true_node;
	}
	if (!is_empty(aux_stack))
		return NULL;
	return sh_node;
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
