#include <pthread.h>

typedef unsigned int cursor_t;

typedef struct shared_graph_header{
	struct graph * mem_adress;	
	int fd;
	int size;
}shared_graph_header;

typedef struct status{
	long mtype;
	int mem[1000], 
		cursor, 
		flag,
		client_id;
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

typedef struct graph{
	struct graph_node * first;
	struct graph_node * current;
/* Mas informacion que pueda tener */
} graph;

typedef struct ipc_params{

	/*------------------*/
	/* GENERAL PARAMS */

	int unique_id;

	/*------------------*/

	/*------------------*/
	/* FIFO PARAMS */
	char * file;
	int action;
	int fd;

	/*------------------*/

	/* SHARED MEMORY PARAMS */
	
	int semid;
	cursor_t shmem_name;
	int shm_segment_size;
	int segment_id;
	void* shared_memory_address;
	
	//char* msg;

	/*------------------*/
	/* MESSAGE QUEUE PARAMS */

	int unique_mq_id;
	int msg_type;

	/*------------------*/

	/*------------------*/
	/* SOCKET PARAMS */

	char* socket_name;
	unsigned int sockfd;
	unsigned int client_sockfd;

	/*------------------*/

	/*------------------*/

} ipc_params;

typedef struct process{
	struct ipc_params * params;
	int type;
	int pid;
} process;

typedef struct process_params{
	struct status * c_status;
	int param;
	struct graph * sh_graph;
} process_params;

typedef struct client_header{
	int client_id;
	int program_size;
} client_header;

typedef status * status_t;
typedef process * process_t;
typedef instruction * instruction_t;
typedef stack * mstack_t;
typedef node * stack_node;
typedef graph_node * node_t;
typedef graph * graph_t;
typedef ipc_params * ipc_params_t;
typedef shared_graph_header * shared_graph_header_t;
typedef process_params * process_params_t;
typedef client_header * client_header_t;
