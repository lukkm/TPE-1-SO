#include <stdlib.h>
#include <string.h>

#include "../../include/structs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_structures/stack.h"
#include "../../include/defs.h"

int parse_string(char *, mstack_t);

graph_t build_graph(mstack_t instructions)
{
    graph_t new_graph = (graph_t)calloc(1, sizeof(graph));
    graph_t expr_graph;

    mstack_t aux_node_stack = (mstack_t)calloc(1, sizeof(stack));
    mstack_t expr_stack;

    node_t new_node;
    node_t ending_graph_node;
	node_t aux_cond_node;

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
		new_node->cond_executed = 0;
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
			expr_stack = create_stack();

			if (parse_string(new_instruction->expr, expr_stack) == -1)
				return NULL;
			
			expr_graph = build_graph(expr_stack);
			if (expr_graph == NULL)
				return NULL;
			new_node->conditional_expr = expr_graph->first;
			aux_cond_node = expr_graph->first;
			while (aux_cond_node->true_node != NULL)
				aux_cond_node = aux_cond_node->true_node;
			aux_cond_node->true_node = new_node;
        }
            
        new_graph->first = new_node;
        free(aux_node);
    }
    if (!is_empty(aux_node_stack))
		return NULL;
    return new_graph;
}

int get_params_size(ipc_params_t params)
{
	int str_size = 0;
	if (params == NULL)
		return 0;

	if (params->file != NULL)
		str_size += strlen(params->file) + 1;
	/*if (params->shared_memory_address != NULL)
		str_size += strlen(params->shared_memory_address)+1;*/
	return sizeof(ipc_params) + str_size*sizeof(char);
}

int get_process_t_size(process_t proc)
{
	if (proc == NULL)
		return 0;
	return sizeof(process) + get_params_size(proc->params);
}

int get_instr_size(instruction_t instr)
{
	int str_size = 0;
	if (instr == NULL)
		return 0;

	if (instr->expr != NULL);
		//str_size = strlen(instr->expr)*sizeof(char)+1;
		
	return sizeof(instruction) + str_size +
			get_process_t_size(instr->instruction_type);
}

int get_node_size(node_t cur_node)
{
	int cond_size = 0;
	int true_size = 0;
	if (cur_node == NULL)
		return 0;
	if (cur_node->conditional_expr != NULL)
	{
		if (!cur_node->cond_executed)
		{
			cur_node->cond_executed = 1;
			cond_size = get_node_size(cur_node->conditional_expr);
		} else {
			cur_node->cond_executed = 0;
			return 0;
		}
	}
	if (cur_node->instruction_process->instruction_type->type == WHILE){
		if (!cur_node->while_executed)
		{
			cur_node->while_executed = 1;
			true_size = get_node_size(cur_node->true_node);
			return get_node_size(cur_node->false_node) + 
			cond_size + true_size + 
			sizeof(graph_node) + 
			get_instr_size(cur_node->instruction_process);
		} else {
			cur_node->while_executed = 0;
			return 0;
		}
	}
	return get_node_size(cur_node->true_node) + 
			cond_size + 
			sizeof(graph_node) + 
			get_instr_size(cur_node->instruction_process);
}

int get_graph_size(graph_t c_graph)
{	
	return get_node_size(c_graph->first) + sizeof(graph);
}

void free_node_space(node_t c_node, int is_conditional){
	if(is_conditional)
		free(c_node->instruction_process->expr);
	free(c_node->instruction_process);
	free(c_node);
}

void free_node(node_t c_node){
	if(c_node == NULL)
		return;
	int instr_type = c_node->instruction_process->instruction_type->type;
	if(instr_type == IF || instr_type == WHILE){
		if(!c_node->cond_executed){
			c_node->cond_executed = 1;
			free_node(c_node->conditional_expr);
		}else{
			if(instr_type == WHILE){
				if(!c_node->while_executed){
					c_node->while_executed = 1;
					free_node(c_node->true_node);
				}else{
					free_node(c_node->false_node);
					free_node_space(c_node, TRUE);
				}
			}else{
				free_node(c_node->true_node);
				free_node_space(c_node, TRUE);
			}
		}
	}else{
		free_node(c_node->true_node);
		free_node_space(c_node, FALSE);
	}
}

void free_graph(graph_t c_graph){
	free_node(c_graph->first);
	free(c_graph);
}
