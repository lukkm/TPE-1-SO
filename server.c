#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


#include "structs.h"
#include "parser.h"
#include "defs.h"
#include "Data_Structures/graph.h"
#include "Data_Structures/stack.h"
#include "IPCS/ipcs.h"

process_t inc_process;
process_t dec_process;
process_t mr_process;
process_t ml_process;
process_t cz_process;
process_t if_process;
process_t endif_process;
process_t while_process;
process_t endwhile_process;

process_t * process_list[CANT_INSTRUCTIONS];
char * process_name_list[CANT_INSTRUCTIONS];

void init(void);
void create_processes(void);
void fatal(char *s);
void create_sh_graph(graph_t, int, int);
int get_graph_size(graph_t);

int
main(void) 
{
	init();
	stack_t c_stack;
	graph_t c_graph;

	char * read_string;
	int aux_size;
	
	printf("holi\n");
	
	client_header_t header = calloc(1, sizeof(struct client_header));


	ipc_params_t server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = "/tmp/server";
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	while(1){
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0){
			
			read_string = calloc(1, header->program_size);
			
			sleep(1);
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0){
				sleep(1);
			} 

			c_stack = parse_file(read_string);

			c_graph = build_graph(c_stack);

			if (c_graph != NULL){
				create_sh_graph(c_graph, get_graph_size(c_graph), 
								190690);
			}else{
				printf("Escribi bien, pelotudo\n");
			}
		sleep(1);
		}
	}
		

	return 0;
}

void create_sh_graph(graph_t c_graph, int size, int memkey)
{
	graph_t sh_graph;
	int memid;
	void * mem;
	
	if ( (memid = shmget(memkey, size, IPC_CREAT|0666)) == -1 )
		fatal("shmget");
	if ( !(mem = shmat(memid, NULL, 0)) )
		fatal("shmat");
	if (copy_graph(c_graph, mem, size) == size);
		printf("BIEEEEN CAMPEON DEL MUNDO, BARRILETE COSMICO\n");
	sh_graph = (graph_t)mem;
	// CORRER PASANDOLE memid	
}

void * copy_node(node_t cur_node, int start, int size)
{
	return 0;
}

void * copy_conditional_branch(graph_t sh_graph, 

int copy_graph(graph_t c_graph, void * sh_graph, int mem_size)
{
	int cursor = 0, aux_size, aux_type;
	node_t aux_sh_node, aux_c_node;
	aux_size = sizeof(graph);
	
	sh_graph = memcpy(sh_graph, c_graph, aux_size);
	cursor += aux_size;
	
	aux_size = get_node_size(c_graph->first);
	if ((((graph_t)sh_graph)->first = 
				copy_node(sh_graph, c_graph->first, cursor, aux_size)) == -1)
		return -1;
	cursor += aux_size;
	((graph_t)sh_graph)->current = ((graph_t)sh_graph)->first;
	
	aux_sh_node = ((graph_t)sh_graph)->first;
	aux_c_node = c_graph->first;
	
	while (cursor <= mem_size)
	{
		aux_size = get_node_size(aux_c_node);
		if ((aux_sh_node = copy_node(sh_graph, aux_c_node, cursor, 
										aux_size)) != -1)
			return -1;
		cursor += aux_size;
		if (flag)
		{
			*((node_t*)pop(STACK)) = aux_node;
			flag = 0;
		}
		aux_type = 
				aux_c_node->instruction_process->instruction_type->type;
		if (aux_type == ENDIF || aux_type == ENDWHILE)
			flag = 1;
		if (aux_c_node->false_node != NULL)
			push(STACK, &aux_sh_node->false_node);
		if (aux_c_node->conditional_expr != NULL)
			aux_sh_node->conditional_expr = 
					copy_conditional_branch(sh_graph, 
								aux_c_node->conditional_expr, &cursor);
		aux_sh_node->true_node = sh_graph+cursor;
	}
	if (!is_empty(STACK))
		return -1;
	return cursor;
}

void init(){
	int i;

	process_list[0] = &inc_process;
	process_list[1] = &dec_process; 
	process_list[2] = &mr_process;
	process_list[3] = &ml_process;
	process_list[4] = &cz_process;
	process_list[5] = &if_process;
	process_list[6] = &endif_process;
	process_list[7] = &while_process; 
	process_list[8] = &endwhile_process;
	
	process_name_list[0] = "inc";
	process_name_list[1] = "dec";
	process_name_list[2] = "mr";
	process_name_list[3] = "ml";
	process_name_list[4] = "cz";
	process_name_list[5] = "if";
	process_name_list[6] = "endif";
	process_name_list[7] = "while";
	process_name_list[8] = "endwhile";

	for (i = 0; i < CANT_INSTRUCTIONS; i++){
		*(process_list[i]) = calloc(1, sizeof(struct process));
		(*(process_list[i]))->params = calloc(1, sizeof(struct ipc_params));
	}

	inc_process->type = INC;
	dec_process->type = DEC;
	mr_process->type = MR;
	ml_process->type = ML;
	cz_process->type = CZ;
	if_process->type = IF;
	endif_process->type = ENDIF;
	while_process->type = WHILE;	
	endwhile_process->type = ENDWHILE;	

	create_processes();
}

void create_processes(){
	int i, j, pid;
	int string_length;
	int to_exec_length;
	char * string_name;
	char * ct_tmp = "/tmp/";
	char * ct_process = "Processes/";
	char * to_exec;
	
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		
		
		(*(process_list[i]))->params = calloc(1, sizeof(struct ipc_params));
		string_length = strlen(process_name_list[i]) +  TMP_LENGTH + 1;
		string_name = calloc(1, string_length);
		for (j = 0; j < TMP_LENGTH; j++)
			string_name[j] = ct_tmp[j];
		for (j = 0; j < string_length - TMP_LENGTH; j++)
			string_name[j+TMP_LENGTH] = process_name_list[i][j];
		string_name[string_length - 1] = 0;
		(*(process_list[i]))->params->file = string_name;
		ipc_create((*(process_list[i]))->params);
		
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
				for (j = 0; j < string_length - TMP_LENGTH; j++)
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
