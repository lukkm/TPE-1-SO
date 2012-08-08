#include <stdlib.c>

#include "stack.h"


stack_t create_stack(){
	return calloc(sizeof(struct stack));
}

int push(stack_t stack, void * info){
	if (stack == NULL)
		return -1;

	stack_node new_node = calloc(sizeof(struct node));
	
	if (new_node == NULL)
		return -1;

	new_node->info = info;	
	new_node->next = stack->first;
	stack->first = new_node;
	(stack->size)++;
}

stack_node pop(stack_t stack){
	if (isEmpty(stack))
		return NULL;

	stack_node ans = stack->first;
	stack->first = stack->first->next;
	(stack->size)--;	
	return ans;	
}

int isEmpty(stack_t stack){
	return stack->size == 0;
}
