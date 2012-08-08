#define MEM_SIZE 1000

typedef struct program{
	int * memory;
	int current = 0;
	int variable;
}program_t;

typedef struct process{
	int pid;
	/* Aca va la informacion del proceso para IPC */
}process_t;

typedef struct graph_node{
	process_t instruction_process;
	node_t true_node;
	node_t false_node;
}node_t;


