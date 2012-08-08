struct program{
	int * memory;
	int current = 0;
	int variable;
};
typedef struct program * program_t;

struct process{
	/* Info de la IPC */
	int conditional;
	int type;
};
typedef struct process * process_t;

struct instruction {
	process_t instruction_type;
	int param;
	char * expr;
};
typedef struct instruction * instruction_t;


