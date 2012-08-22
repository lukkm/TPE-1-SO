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

node_t copy_graph(node_t, void *, int *);

graph_t create_sh_graph(graph_t c_graph, int size, int * memkey, shared_graph_header_t header)
{
	graph_t sh_graph;
	int cursor;
	void * mem;
	
	if ( (long)(*memkey = shmget(IPC_PRIVATE, size, IPC_CREAT|0666)) == -1 )
		fatal("shmget");
	if ( (long)(mem = shmat(*memkey, NULL, 0)) == -1 )
		fatal("shmat");
	header->fd = *memkey;
	header->mem_adress = mem; 
	header->size = size;
	if ((cursor = w_copy_graph(c_graph, mem)) != size)
		return NULL;
	sh_graph = (graph_t)mem;
	return sh_graph;
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
		if (aux_c_node->true_node != NULL)
			aux_sh_node->true_node = sh_graph + *cursor;
		else
			aux_sh_node->true_node = NULL;
		aux_sh_node = aux_sh_node->true_node;
		aux_c_node = aux_c_node->true_node;
	}
	if (!is_empty(aux_stack))
		return NULL;
	return sh_node;
}
