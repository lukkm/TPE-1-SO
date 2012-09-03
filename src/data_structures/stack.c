#include <stdlib.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/data_structures/stack.h"


mstack_t create_stack(){
	return (mstack_t)calloc(1, sizeof(stack));
}

int push(mstack_t c_stack, void * info){
	if (c_stack == NULL)
		return -1;

	stack_node new_node = (stack_node)calloc(1, sizeof(node));
	
	if (new_node == NULL)
		return -1;

	new_node->info = info;	
	new_node->next = c_stack->first;
	c_stack->first = new_node;
	(c_stack->size)++;
}

stack_node pop(mstack_t c_stack){
	if (is_empty(c_stack))
		return NULL;

	stack_node ans = c_stack->first;
	c_stack->first = c_stack->first->next;
	(c_stack->size)--;	
	return ans;	
}

int is_empty(mstack_t c_stack){
	if (c_stack == NULL)
		return TRUE;
	return c_stack->size == 0;
}
