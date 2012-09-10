#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"

void create_processes();
void create_processes_information(int);

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

process_t * process_list[CANT_INSTRUCTIONS];
char * process_name_list[CANT_INSTRUCTIONS];

int init_sem(int sem_doinit){
	
	int semid, i;
	
	if ( (semid = semget(190690, SEMSET_SIZE, IPC_CREAT | 0666)) == -1 )
		exit(1);
		
	if(sem_doinit){
		printf("init\n");
		for ( i = 0; i < SEMSET_SIZE; i++ )
		{
			if (i % 2 == 0)
				semctl(semid, i, SETVAL, 1);
			else
				semctl(semid, i, SETVAL, 0);
		}
	}
		
	/*for ( i = 0; i < SEMSET_SIZE; i++ )
		printf("%d ", semctl(semid, i, GETVAL));
	printf("\n");*/
	
	return semid;
}

void init_processes(int sem_doinit){
	
	int i, aux_semid;
	
	char * ct_sv_rec_params = "/tmp/sv_receive";
	char * ct_sv_params = "/tmp/server";
	
	aux_semid = init_sem(sem_doinit);
	
	server_receive_params = calloc(1, sizeof(struct ipc_params));
	server_receive_params->file = calloc(1, strlen(ct_sv_rec_params) + 1);
	strcpy(server_receive_params->file, ct_sv_rec_params);
	server_receive_params->shm_segment_size = sizeof(struct status);
	server_receive_params->unique_id = 30000;
	server_receive_params->msg_type = PROGRAM_STATUS;
	server_receive_params->wr_sem = SVR_RCV_WR;
	server_receive_params->rd_sem = SVR_RCV_RD;
	server_receive_params->semid = aux_semid;
	server_receive_params->sockfd = -1;
	server_receive_params->client_sockfd = -1;
	
	server_params = calloc(1,sizeof(struct ipc_params));
	server_params->file = calloc(1, strlen(ct_sv_params) + 1);
	strcpy(server_params->file, ct_sv_params);
	server_params->shm_segment_size = sizeof(struct client_header) + MAX_PROGRAM_LENGTH;
	server_params->unique_id = 30001;
	server_params->wr_sem = SVR_PAR_WR;
	server_params->rd_sem = SVR_PAR_RD;
	server_params->semid = aux_semid;
	server_params->sockfd = -1;
	server_params->client_sockfd = -1;

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

	for (i = 0; i < CANT_INSTRUCTIONS; i++)
		*(process_list[i]) = calloc(1, sizeof(struct process));

	inc_process->type = INC;
	dec_process->type = DEC;
	mr_process->type = MR;
	ml_process->type = ML;
	cz_process->type = CZ;
	if_process->type = IF;
	endif_process->type = ENDIF;
	while_process->type = WHILE;	
	endwhile_process->type = ENDWHILE;	

	create_processes_information(aux_semid);
		
}

void create_processes_information(int aux_semid){
	int i, j, pid;
	int string_length, sem_cur = SVR_PAR_WR + 1;
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
		(*(process_list[i]))->params->shm_segment_size = sizeof(struct status);
		(*(process_list[i]))->params->unique_id = getpid();
		(*(process_list[i]))->params->msg_type = PROGRAM_STATUS;
		(*(process_list[i]))->params->rd_sem = sem_cur + (i*2);
		(*(process_list[i]))->params->wr_sem = sem_cur + (i*2) + 1;
		(*(process_list[i]))->params->semid = aux_semid;
		(*(process_list[i]))->params->sockfd = -1;
		(*(process_list[i]))->params->client_sockfd = -1;
	}
}

void free_processes() {
	free(server_receive_params->file);
	free(server_receive_params);
	
	free(server_params->file);
	free(server_params);
	
	for (i = 0; i < CANT_INSTRUCTIONS; i++) {
		free((*(process_list[i]))->params->file);
		free((*(process_list[i]))->params);
		free(*(process_list[i]));
	}
}
