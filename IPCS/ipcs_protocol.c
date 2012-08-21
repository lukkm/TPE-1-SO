#include <stdlib.h>
#include <string.h>

#include "../structs.h"
#include "../defs.h"
#include "ipcs.h"

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

process_t * process_list[CANT_INSTRUCTIONS];
char * process_name_list[CANT_INSTRUCTIONS];

ipc_params_t ipcs_com_info[CANT_INSTRUCTIONS * CANT_INSTRUCTIONS];

void init_processes(){
	
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
		(*(process_list[i]))->params = calloc(1, sizeof(struct ipc_params));
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
	int to_exec_length;
	char * string_name;
	char * ct_tmp = "/tmp/";
	char * ct_process = "Processes/";
	char * to_exec;
	
	
	for(i = 0; i < CANT_INSTRUCTIONS; i++){
		(*(process_list[i]))->params = calloc(1, sizeof(struct ipc_params));
		string_length = strlen(process_name_list[i]) +  TMP_LENGTH + 1;
		string_name = calloc(1, string_length);
		for (j = 0; j < TMP_LENGTH; j++)
			string_name[j] = ct_tmp[j];
		for (j = 0; j < string_length - TMP_LENGTH; j++)
			string_name[j+TMP_LENGTH] = process_name_list[i][j];
		string_name[string_length - 1] = 0;
		(*(process_list[i]))->params->file = string_name;
	}
}
