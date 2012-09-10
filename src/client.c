#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../include/structs.h"
#include "../include/defs.h"
#include "../include/ipcs/ipcs.h"
#include "../include/utils/process_utils.h"

extern ipc_params_t server_params;

int main(int argc, char ** argv){
	
	char * program_name;
	struct status cl_program;
	int i;
	ipc_params_t client_params;

	init_processes(FALSE);

	if(argc > 1){
		program_name = argv[1];
	}else{
		printf("Entrada incorrecta\n");
		return 0;
	}

	client_header_t header = calloc(1, sizeof(struct client_header));
	header->program_size = strlen(program_name);
	header->client_id = getpid();

	server_params->socklistener = 1;

	ipc_open(server_params, O_WRONLY);
	
	server_params->msg_type = PRE_HEADER;
	//printf("%d %d\n", server_params->wr_sem, server_params->rd_sem);
	ipc_send(server_params, header, sizeof(struct client_header));
	
	//free(header);
	
	server_params->msg_type = PROGRAM_STRING;
	ipc_send(server_params, program_name, header->program_size);
	
	ipc_close(server_params);
	
	client_params = get_params_from_pid(getpid(), PROGRAM_STATUS, sizeof(struct status), server_params->semid);
	
	ipc_create(client_params);
	ipc_open(client_params, O_RDONLY);
	while (!ipc_receive(client_params, &cl_program, sizeof(struct status)));
	ipc_close(client_params);
	ipc_destroy(client_params);
	
	if (cl_program.flag == -1){
		printf("File error\n");
	}else{
		printf("\nResult for program %d: \n", getpid());
		printf("--------------------------\n");
		printf("Flag: %s\n", cl_program.flag ? "True" : "False");
		printf("--------------------------\n");
		printf("Memory status:\n");
		for (i = 0; i < MEM_SIZE; i++){
			printf("%d ", cl_program.mem[i]);
		}
		printf("\n\n");
	}

	return 0;
}
