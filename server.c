#include <stdio.h>

#include "structs.h"
#include "parser.h"
#include "Processes/process_defs.h"
#include "Data_Structures/graph.h"
#include "Data_Structures/stack.h"

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
	init();
	stack_t c_stack;
	graph_t c_graph;

	node_t c_g_node;
	
	c_stack = parse_file("hola");

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
		printf("Escribi bien, pelotudo");
	}
		

	return 0;
}

void init(){

	inc_process = calloc(sizeof(struct process));
	dec_process = calloc(sizeof(struct process));
	mr_process = calloc(sizeof(struct process));
	ml_process = calloc(sizeof(struct process));
	cz_process = calloc(sizeof(struct process));
	if_process = calloc(sizeof(struct process));
	endif_process = calloc(sizeof(struct process));
	while_process = calloc(sizeof(struct process));
	endwhile_process = calloc(sizeof(struct process));

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
