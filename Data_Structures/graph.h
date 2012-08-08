#include "../structs.c"

typedef struct graph_node * node_t;
struct graph_node{
	process_t instruction_process;
	node_t true_node;
	node_t false_node;
};

struct graph{
	node_t first;
	/* Mas informacion que pueda tener */
};
typedef graph * graph_t;

graph_t build_graph();



