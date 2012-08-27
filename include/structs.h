typedef struct shared_graph_header{
	struct graph * mem_adress;	
	int fd;
	int size;
}shared_graph_header;

typedef struct status{
	long mtype;
	int mem[1000];
	int cursor;
	int flag;
	struct shared_graph_header g_header;
} status;

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
	struct graph_node * conditional_expr;
	int cond_executed;
	int while_executed;
} graph_node;

typedef struct{
	struct graph_node * first;
	struct graph_node * current;
/* Mas informacion que pueda tener */
} graph;

typedef struct process_params{
	struct status * c_status;
	int param;
} process_params;

typedef struct ipc_params{

	/*------------------*/
	/* FIFO PARAMS */
	char * file;
	int action;
	int fd;

	/*------------------*/

	/* SHARED MEMORY PARAMS */
	int shm_segment_size;
	int segment_id;
	void* shared_memory_address; // IDIOTA VOS PUTO!!! //EL LUKO DIJO QUE ERA VOID
	//char* msg;

	/*------------------*/
	/* MESSAGE QUEUE PARAMS */

	int mq_id;
	int unique_mq_id;

	/*------------------*/

	/*------------------*/
	/* SOCKET PARAMS */

	int socket_fd;

	/*------------------*/

} ipc_params;

typedef struct process{
	struct ipc_params * params;
	int type;
	int pid;
} process;

typedef struct client_header{
	int client_id;
	int program_size;
} client_header;


typedef process_params * process_params_t;
typedef status * status_t;
typedef process * process_t;
typedef instruction * instruction_t;
typedef stack * stack_t;
typedef node * stack_node;
typedef graph_node * node_t;
typedef graph * graph_t;
typedef ipc_params * ipc_params_t;
typedef shared_graph_header * shared_graph_header_t;
typedef client_header * client_header_t;
