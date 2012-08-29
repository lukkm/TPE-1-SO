#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#include "../../include/structs.h"
#include "../../include/utils/parser.h"
#include "../../include/defs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_structures/stack.h"
#include "../../include/ipcs/ipcs.h"

typedef struct{
	long mtype_st;
	struct status info_st;
} status_msg;

typedef struct{
	long mtype_cl;
	struct client_header info_cl;
} client_msg;

typedef struct{
	long mtype_pr;
	char info_pr[MAX_PROGRAM_LENGTH];
} string_msg;

status_msg new_msg_st;
client_msg new_msg_cl;
string_msg new_msg_pr;

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void ipc_create(ipc_params_t params){	
		/*printf("Conectado a la única Message Queue ID: %d\n", params->unique_mq_id);*/
}

void ipc_open(ipc_params_t params, int action){
	
	//IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR
	
	//struct msqid_ds sets;
	
	if ((params->unique_mq_id = msgget((key_t)params->unique_id, IPC_CREAT | 0666)) == -1)
		perror("Error in MessageQueue Allocation");  //VER PERMISOS
}

void ipc_send(ipc_params_t params, void * message, int size)
{
	void * new_msg;
	int msg, msg_size, i = 0;
	char * aux_str;
	
	switch (params->msg_type)
	{
		case PRE_HEADER:
			new_msg_cl.mtype_cl = params->unique_id;
			new_msg_cl.info_cl.program_size = 
							((client_header_t)message)->program_size;
			new_msg_cl.info_cl.client_id = 
							((client_header_t)message)->client_id;
			new_msg = &new_msg_cl;
			msg_size = sizeof(client_msg);
			break;
		case PROGRAM_STRING:
			aux_str = message;
			new_msg_pr.mtype_pr = params->unique_id;
			while (aux_str[i] != 0) {
				new_msg_pr.info_pr[i] = aux_str[i];
				i++;
			}
			new_msg_pr.info_pr[i] = 0;
			new_msg = &new_msg_pr;
			msg_size = sizeof(string_msg);
			break;
		case PROGRAM_STATUS:
			new_msg_st.mtype_st = params->unique_id;
			new_msg_st.info_st = *((status_t)message);
			new_msg_st.info_st.g_header = ((status_t)message)->g_header;
			new_msg = &new_msg_st;
			msg_size = sizeof(status_msg);
			break;
		default:
			return;
	}
		
	if (!params->unique_mq_id)
	{
		perror("Error MsgQueue not set\n");
		exit(1);
	}
	
	msg = msgsnd(params->unique_mq_id, new_msg, 
				msg_size, 0);
	
	if (msg)
		printf("Code: %d %d\n",msg, errno);
}

int ipc_receive(ipc_params_t params, void * buffer, int size){
	
	int rec = 0;	
	void * info;

	switch (params->msg_type)
	{
		case PRE_HEADER:
			new_msg_cl.mtype_cl = params->unique_id;
			rec = msgrcv(params->unique_mq_id, &new_msg_cl, 
						sizeof(client_msg), 0, MSG_NOERROR);
			info = &new_msg_cl.info_cl;
			break;
		case PROGRAM_STRING:
			new_msg_pr.mtype_pr = params->unique_id;
			rec = msgrcv(params->unique_mq_id, &new_msg_pr, 
						sizeof(string_msg), 0, MSG_NOERROR);
			info = &new_msg_pr.info_pr;
			break;
		case PROGRAM_STATUS:
			new_msg_st.mtype_st = params->unique_id;
			rec = msgrcv(params->unique_mq_id, &new_msg_st, 
						sizeof(status_msg), 0, MSG_NOERROR);
			info = &new_msg_st.info_st;
			break;
		default:
			rec = 0;
	}
	
	if (rec)
		memcpy(buffer, info, size);

	return rec;
}

void ipc_close(ipc_params_t params){ }

void ipc_destroy(ipc_params_t params)
{
	msgctl(params->unique_mq_id, IPC_RMID, NULL);
}
