#include "../structs.c" 

struct stack{
	stack_node first;
	int size;
};
typedef stack * stack_t;

struct node{
	stack_node next;
	void * info;
	/* Informacion del nodo, puede ser process_t o node_t */
}; 

typedef struct node * stack_node;

stack_t create_stack();
int push(stack_node stack, void * node);
stack_node pop(stack_t stack);
int is_empty(stack_t stack);

