#include "graph.h"
#include "stack.h"
#include "../defs.h"
#include <stdlib.h>

graph_t build_graph(stack_t instructions)
{
    graph_t new_graph = calloc(sizeof(*graph_t));
    stack_t aux_node_stack = calloc(sizeof(*stack_t));
    stack_node aux_node;
    node_t new_node;
    node_t ending_node;
    int new_node_type;
    
    while (!is_empty(instructions))
    {
        aux_node = pop(instructions);
        
        new_node_type = new_node->instruction_process->instruction_type->type;
        
        new_node->instruction_process = (instruction_t)aux_node->info;
        if (new_graph->first)
                new_node->true_node = new_graph->first;
        if (new_node_type == ENDWHILE || new_node_type == ENDIF)
            push(aux_node_stack, new_node);
        else if (new_node_type == WHILE || new_node_type == IF)
        {
            ending_node = pop(aux_node_stack);
            if (ending_node->instruction_process->param != 
                    new_node->instruction_process->param)
                return NULL;
            new_node->false_node = ending_node->true_node;
            if (new_node_type == WHILE)
                ending_node->true_node = new_node;
        }
            
        new_graph->first = new_node;
    }
    return new_graph;
}