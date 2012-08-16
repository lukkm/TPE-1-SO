#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


#include "structs.h"
#include "parser.h"
#include "defs.h"
#include "Data_Structures/graph.h"
#include "Data_Structures/stack.h"
#include "IPCS/ipcs.h"

process_t inc_process;
process_t dec_process;
process_t mr_process;
process_t ml_process;
process_t cz_process;
process_t if_process;
process_t endif_process;
process_t while_process;
process_t endwhile_process;

void init(void);
void fatal(char *s);
void create_sh_graph(graph_t, int, int);
int get_graph_size(graph_t);

int
main(void) 
{
	init();
	stack_t c_stack;
	graph_t c_graph;

	char * read_string;
	int aux_size;
	
	client_header_t header = calloc(1, sizeof(struct client_header));


	ipc_params_t server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = "/tmp/server";
	ipc_create(server_params);
	ipc_open(server_params, O_RDONLY);
	while(1){
		if(ipc_receive(server_params, header, sizeof (struct client_header)) > 0){
			
			read_string = calloc(1, header->program_size);
			
			sleep(1);
			
			while(ipc_receive(server_params, read_string, header->program_size) == 0){
				sleep(1);
			} 

			c_stack = parse_file(read_string);

			c_graph = build_graph(c_stack);

			if (c_graph != NULL){
				create_sh_graph(c_graph, aux_size, 190690);
			}else{
				printf("Escribi bien, pelotudo\n");
			}
		sleep(1);
		}
	}
		

	return 0;
}

void create_sh_graph(graph_t c_graph, int size, int memkey)
{
	graph_t sh_graph;
	int memid;
	void * mem;
	
	if ( (memid = shmget(memkey, size, IPC_CREAT|0666)) == -1 )
		fatal("shmget");
	if ( !(mem = shmat(memid, NULL, 0)) )
		fatal("shmat");
	sh_graph = (graph_t)mem;
	*sh_graph = *c_graph;
	// CORRER PASANDOLE memid	
}

void init(){

	process_t * process_list[CANT_INSTRUCTIONS];
	int i;

	process_list[0] = &inc_process;
	process_list[1] = &dec_process; 
	process_list[2] = &mr_process;
	process_list[3] = &ml_process;
	process_list[4] = &cz_process;
	process_list[5] = &if_process;
	process_list[6] = &endif_process;
	process_list[7] = &while_process; 
	process_list[8] = &endwhile_process;

	for (i = 0; i < CANT_INSTRUCTIONS; i++){
		*(process_list[i]) = calloc(1, sizeof(struct process));
	}

	inc_process->type = INC;
	dec_process->type = DEC;
	mr_process->type = MR;
	ml_process->type = ML;
	cz_process->type = CZ;
	if_process->type = IF;
	endif_process->type = ENDIF;
	while_process->type = WHILE;	
	endwhile_process->type = ENDWHILE;	

	//create_processes();
}
