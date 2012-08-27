#include <stdlib.h>
#include <string.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"

void create_processes();
void create_processes_information();

int i;

process_t inc_process;
process_t dec_process;
process_t mr_process;
process_t ml_process;
process_t cz_process;
process_t if_process;
process_t endif_process;
process_t while_process;
process_t endwhile_process;

ipc_params_t server_receive_params;
ipc_params_t server_params;
ipc_params_t client_params;

process_t * process_list[CANT_INSTRUCTIONS];
char * process_name_list[CANT_INSTRUCTIONS];

void init_processes(){
	
	int i;
	
	char * ct_sv_rec_params = "/tmp/sv_receive";
	char * ct_sv_params = "/tmp/server";
	char * ct_cl_params = "/tmp/client";
	
	server_receive_params = calloc(1, sizeof(struct ipc_params));
	server_receive_params->file = calloc(1, strlen(ct_sv_rec_params) + 1);
	strcpy(server_receive_params->file, ct_sv_rec_params);
	server_receive_params->mq_id = 30000;
	
	server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = calloc(1, strlen(ct_sv_params) + 1);
	strcpy(server_params->file, ct_sv_params);
	server_params->mq_id = 30001;

	client_params = calloc(1,sizeof(struct ipc_params));
	client_params->file = calloc(1, strlen(ct_cl_params) + 1);
	strcpy(client_params->file, ct_cl_params);
	client_params->mq_id = 30002;

	process_list[0] = &inc_process;
	process_list[1] = &dec_process; 
	process_list[2] = &mr_process;
	process_list[3] = &ml_process;
	process_list[4] = &cz_process;
	process_list[5] = &if_process;
	process_list[6] = &endif_process;
	process_list[7] = &while_process; 
	process_list[8] = &endwhile_process;
	
	process_name_list[0] = "inc";
	process_name_list[1] = "dec";
	process_name_list[2] = "mr";
	process_name_list[3] = "ml";
	process_name_list[4] = "cz";
	process_name_list[5] = "if";
	process_name_list[6] = "endif";
	process_name_list[7] = "while";
	process_name_list[8] = "endwhile";

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

	create_processes_information();
		
}

void create_processes_information(){
	int i, j, pid;
	int string_length;
	char * string_name;
	char * ct_tmp = "/tmp/";
	
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		(*(process_list[i]))->params = calloc(1, sizeof(struct ipc_params));
		string_length = strlen(process_name_list[i]) +  TMP_LENGTH + 1;
		string_name = calloc(1, string_length);
		strcpy(string_name, ct_tmp);
		strcat(string_name, process_name_list[i]);
		string_name[string_length - 1] = 0;
		(*(process_list[i]))->params->file = string_name;
		(*(process_list[i]))->params->mq_id = 40000+i;
	}
}
