#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


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

void init(void);

int
main(void) 
{
	printf("hola");
	init();
	stack_t c_stack;
	graph_t c_graph;
	node_t c_g_node;

	char read_string[100];

	ipc_params_t server_params = calloc(1, sizeof(struct ipc_params));
	server_params->file = "/tmp/server";
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY|O_NONBLOCK);
	while(1){
		if(ipc_receive(server_params, read_string, 100) > 0){
			printf("%s \n", read_string);
		}
		sleep(1);
	}
	
	//c_stack = parse_file("hola");
/*
	c_graph = build_graph(c_stack);

	if (c_graph != NULL){

		c_g_node = c_graph->first;

		do{
	
			printf("%d ", c_g_node->instruction_process->instruction_type->type);
			printf("%d\n", c_g_node->instruction_process->param);
			
			if (c_g_node->false_node != NULL){
				printf("parte falsa del %d: %d\n", c_g_node->instruction_process->instruction_type->type, 
								c_g_node->false_node->instruction_process->instruction_type->type);
			}

			c_g_node = c_g_node->true_node;
		}while(c_g_node != NULL);

	}else{
		printf("Escribi bien, pelotudo\n");
	}
		
*/
	return 0;
}

void init(){

	inc_process = calloc(1, sizeof(struct process));
	dec_process = calloc(1, sizeof(struct process));
	mr_process = calloc(1, sizeof(struct process));
	ml_process = calloc(1, sizeof(struct process));
	cz_process = calloc(1, sizeof(struct process));
	if_process = calloc(1, sizeof(struct process));
	endif_process = calloc(1, sizeof(struct process));
	while_process = calloc(1, sizeof(struct process));
	endwhile_process = calloc(1, sizeof(struct process));

	inc_process->type = INC;
	dec_process->type = DEC;
	mr_process->type = MR;
	ml_process->type = ML;
	cz_process->type = CZ;
	if_process->type = IF;
	endif_process->type = ENDIF;
	while_process->type = WHILE;	
	endwhile_process->type = ENDWHILE;	
}
