/*typedef struct program program;
typedef struct process process;
typedef struct instruction instruction;
typedef struct stack stack;
typedef struct node node;
typedef struct graph_node graph_node;
typedef struct graph graph;*/

typedef struct program{
	int * memory;
	int current;
	int variable;
} program;

typedef struct process{
	/* Info de la IPC */
	int type;
} process;


typedef struct instruction{
	struct process * instruction_type;
	int param;
	char * expr;
} instruction;

typedef struct node{
	struct node * next;
	void * info;
	/* Informacion del nodo, puede ser process_t o node_t */
} node;

typedef struct stack{
	node * first;
	int size;
} stack;

typedef struct graph_node{
	struct instruction * instruction_process;
	struct graph_node * true_node;
	struct graph_node * false_node;
} graph_node;

typedef struct{
	struct graph_node * first;
	/* Mas informacion que pueda tener */
} graph;

typedef struct{
	int cursor;
	int mem[1000];
	int flag;
	int id;
} status;

typedef program * program_t;
typedef process * process_t;
typedef instruction * instruction_t;
typedef stack * stack_t;
typedef node * stack_node;
typedef graph_node * node_t;
typedef graph * graph_t;
typedef status * status_t;
