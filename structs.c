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


