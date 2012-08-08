#define MEM_SIZE 1000

struct program{
	int * memory;
	int current = 0;
	int variable;
}
typedef struct program * program_t;

struct process{
	int pid;
	int instr_id;
	/* Aca va la informacion del proceso para IPC */
}
typedef struct process * process_t;

struct graph_node{
	process_t instruction_process;
	node_t true_node;
	node_t false_node;
}
typedef struct graph_node * node_t;


