#include "../structs.h"
#include "graph.h"
#include "stack.h"
#include "../defs.h"
#include <stdlib.h>

stack_t parse_string(char *);

graph_t build_graph(stack_t instructions)
{
    graph_t new_graph = (graph_t)calloc(1, sizeof(graph));
    graph_t expr_graph;

    stack_t aux_node_stack = (stack_t)calloc(1, sizeof(stack));
    stack_t expr_stack;

    node_t new_node;
    node_t ending_graph_node;

    stack_node aux_node;
    stack_node ending_node;

    instruction_t new_instruction;
    
    int new_node_type;
    int aux_node_type;

    if(instructions == NULL || is_empty(instructions)){
    	return NULL;
    }
    
    while (!is_empty(instructions))
    {

	new_node = (node_t)calloc(1, sizeof(graph_node));

        aux_node = pop(instructions);
        new_instruction = (instruction_t)aux_node->info;
	new_node_type = new_instruction->instruction_type->type;
        
        new_node->instruction_process = new_instruction;
        if (new_graph->first)
                new_node->true_node = new_graph->first;
        if (new_node_type == ENDWHILE || new_node_type == ENDIF)
            push(aux_node_stack, new_node);
        else if (new_node_type == WHILE || new_node_type == IF)
        {
            ending_node = pop(aux_node_stack);
	    ending_graph_node = (node_t)ending_node->info;
	    aux_node_type = ending_graph_node->
				instruction_process->instruction_type->type;
            if (ending_graph_node->instruction_process->param != 
                    new_node->instruction_process->param ||
		aux_node_type == ENDIF && new_node_type != IF ||
		aux_node_type == ENDWHILE && new_node_type != WHILE)
                return NULL;
            new_node->false_node = ending_graph_node->true_node;
            if (new_node_type == WHILE)
                ending_graph_node->true_node = new_node;
	    expr_stack = parse_string(new_instruction->expr);
	    
	    expr_graph = build_graph(expr_stack);
	    if (expr_graph == NULL)
		return NULL;
	    new_node->conditional_expr = expr_graph->first;
        }
            
        new_graph->first = new_node;
    }
    return new_graph;
}
