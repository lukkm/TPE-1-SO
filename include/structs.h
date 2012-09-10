#include <pthread.h>

/*
 *
 * Cursor - entero positivo
 * 
 *  */

typedef unsigned int cursor_t;

/*
 * Header del grafo de memoria compartida.
 * Encapsula un puntero al grafo
 * Un file descriptor, y el tamano.
 * 
 * */

typedef struct shared_graph_header{
	struct graph * mem_adress;	
	int fd;
	int size;
}shared_graph_header;

/*
 * 
 * Representacion del estado.
 * En el se encuentra el vector de 1000 posiciones,
 * el numero del cliente conectado a la porcion de memoria,
 * y un cursor actual sobre dicha porcion.
 * 
 * */

typedef struct status{
	long mtype;
	int mem[1000], 
		cursor, 
		flag,
		client_id;
	struct shared_graph_header g_header;
} status;

/*
 * 
 * Estructura que encapsula la informacion de las instrucciones
 * 
 * */


typedef struct instruction{
		struct process * instruction_type;
		int param;
		char * expr;
} instruction;

/*
 * 
 * Estructura que representa un nodo encadenado,
 * y de contenido generico
 * 
 * */


typedef struct node{
	struct node * next;
	void * info;
	/* Informacion del nodo, puede ser process_t o node_t */
} node;

/*
 * 
 * Estructura que representa una pila, con tamano.
 * 
 * */


typedef struct stack{
	node * first;
	int size;
} stack;

/*
 * 
 * Estructura que especializa al tipo de nodo, en los 3 tipos de nodos.
 * 
 * */


typedef struct graph_node{
	struct instruction * instruction_process;
	struct graph_node * true_node;
	struct graph_node * false_node;
	struct graph_node * conditional_expr;
	int cond_executed;
	int while_executed;
} graph_node;

/*
 * 
 * Estructura que representa un grafo standard.
 * Ademas tiene un puntero a un nodo current del grafo.
 * Importante para el algoritmo de pasaje de procesos.
 * 
 * */


typedef struct graph{
	struct graph_node * first;
	struct graph_node * current;
/* Mas informacion que pueda tener */
} graph;


/*
 * 
 * Parametros de todas las IPC-s
 * 
 * */


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
	cursor_t wr_sem;
	cursor_t rd_sem;
	int shm_segment_size;
	int segment_id;
	void* shared_memory_address;
	

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
	unsigned int socklistener;



} ipc_params;

/*
 * 
 * Estructura que representa un proceso, 
 * con un identificador univoco PID entero
 * 
 * */


typedef struct process{
	struct ipc_params * params;
	int type;
	int pid;
} process;

/*
 * 
 * Estructura que representa los parametros posibles de
 * los procesos.
 * 
 * */


typedef struct process_params{
	struct status * c_status;
	int param;
	struct graph * sh_graph;
} process_params;

/*
 * 
 * Estructura que representa el header del cliente.
 * Identifica univocamente al numero cid del cliente,
 * y almacena el tamano del programa.
 * 
 * 
 * */


typedef struct client_header{
	int client_id;
	int program_size;
} client_header;






/*
 * 
 * Re-definiciones
 * 
 * */
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
