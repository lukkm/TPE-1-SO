#include "structs.h"

struct program{
	int * memory;
	int current;
	int variable;
};

struct process{
	/* Info de la IPC */
	int type;
};


struct instruction {
	process_t instruction_type;
	int param;
	char * expr;
};

struct node{
	struct node * next;
	void * info;
	/* Informacion del nodo, puede ser process_t o node_t */
}; 

struct stack{
	struct node * first;
	int size;
};

struct graph_node{
	instruction_t instruction_process;
	node_t true_node;
	node_t false_node;
};

struct graph{
	node_t first;
	/* Mas informacion que pueda tener */
};

