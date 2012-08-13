#include <stdlib.h>

#include "../structs.h"
#include "../defs.h"
#include "stack.h"


stack_t create_stack(){
	return (stack_t)calloc(1, sizeof(stack));
}

int push(stack_t c_stack, void * info){
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

stack_node pop(stack_t c_stack){
	if (is_empty(c_stack))
		return NULL;

	stack_node ans = c_stack->first;
	c_stack->first = c_stack->first->next;
	(c_stack->size)--;	
	return ans;	
}

int is_empty(stack_t c_stack){
	if (c_stack == NULL)
		return TRUE;
	return c_stack->size == 0;
}
