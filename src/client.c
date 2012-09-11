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
	int i, j;
	ipc_params_t client_params;

	init_processes(FALSE);
	
	printf("\n");

	if(argc <= 1){
		printf("Wrong entry, too few arguments\n");
	}else{

		client_params = get_params_from_pid(getpid(), PROGRAM_STATUS, sizeof(struct status), server_params->semid);
		ipc_create(client_params);

		for (j = 1; j < argc; j++){
			client_header_t header = calloc(1, sizeof(struct client_header));
			
			program_name = argv[j];
			
			header->program_size = strlen(program_name);
			header->client_id = getpid();

			server_params->socklistener = 1;

			ipc_open(server_params, O_WRONLY);
			
			server_params->msg_type = PRE_HEADER;
			ipc_send(server_params, header, sizeof(struct client_header));
			
			server_params->msg_type = PROGRAM_STRING;
			ipc_send(server_params, program_name, header->program_size);
			
			ipc_close(server_params);
			
			ipc_open(client_params, O_RDONLY);
			while (!ipc_receive(client_params, &cl_program, sizeof(struct status)));
			ipc_close(client_params);
			
			
			if (cl_program.flag == FILE_NOT_FOUND){
				printf("File error, no such file \"%s\"\n", program_name);
			}else if (cl_program.flag == UNABLE_TO_BUILD_GRAPH){
				printf("Unable to make graph from program \"%s\"\n", program_name);
			}else if (cl_program.flag < 0){
				printf("File error, wrong syntax in line %d of program \"%s\"\n", (cl_program.flag + 2) * -1, program_name);
			}else{
				printf("\nResult for program \"%s\": \n", program_name);
				printf("--------------------------\n");
				printf("Flag: %s\n", cl_program.flag ? "True" : "False");
				printf("--------------------------\n");
				printf("Memory status:\n");
				for (i = 0; i < MEM_SIZE; i++){
					printf("%d ", cl_program.mem[i]);
				}
				printf("\n\n");
			}
		}
		
		ipc_destroy(client_params);
	}

	return 0;
}
