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

struct msg{
	long mtype;
	char info[MAX_IPC_SIZE];
};

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

/*
int main()
{


 char read_string[100];
 int i = 0;
 ipc_params_t server_params = calloc(1, sizeof(struct ipc_params));
 printf("Message Queue StandAlone - Program \n\n");
 ipc_create_msgqueue(server_params);
 char* msg = "Luko Auriku";
 char* msg2 = "Luko Auriku II";
 char* msg3 = "Luko Auriku III";
 char* msg4 = "Luko Auriku IV";
 ipc_send(server_params,msg,strlen(msg)*sizeof(char));
 ipc_send(server_params,msg2,strlen(msg2)*sizeof(char)); 
 ipc_send(server_params,msg3,strlen(msg3)*sizeof(char));
 ipc_send(server_params,msg4,strlen(msg4)*sizeof(char));
 ipc_open(server_params, 0);
	
     while(1){
		 if(ipc_receive(server_params, read_string, 100) > 0){
			printf("%s \n", read_string);
		   }
		 else
			printf("NO\n", read_string);
		 	sleep(1);
			
					
				
			
		  }
 return 0;
}*/

/*
struct q_msg{
	
 long mtype;
 char mtext[200];
};*/

void ipc_create(ipc_params_t params){	
		/*printf("Conectado a la única Message Queue ID: %d\n", params->unique_mq_id);*/
}

void ipc_open(ipc_params_t params, int action){
	
	//IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR
	
	struct msqid_ds sets;
	sets.msg_qbytes = 32768l;
	
	if ((params->unique_mq_id = msgget((key_t)params->mq_id, IPC_CREAT | 0666)) ==-1)
		perror("Error in MessageQueue Allocation");  //VER PERMISOS
	else			
	 printf("Message Queue alocada en %d \n", params->unique_mq_id);
	msgctl(params->unique_mq_id, IPC_SET, &sets);
	msgctl(params->unique_mq_id, IPC_STAT, &sets);
	printf("Tamanio de la queue: %ul\n", (unsigned long)sets.msg_qbytes);
	return;
}

void ipc_send(ipc_params_t params, void * message, int size){

	struct msg new_msg;
	
	new_msg.mtype = params->unique_mq_id;
	memcpy(new_msg.info, message, size);
		
	if (!params->unique_mq_id)
	{
		perror("Error MessageQueue not set \n");
		exit(1);
	}
	int msg = msgsnd(params->unique_mq_id, &new_msg, size + sizeof(long), 0); //O IPC_NOWAIT , Y EL PROCESO NO SE BLOQUEA SI TIENE QUE ESPERAR
	
	if (msg) 
		printf("Code: %d %d\n",msg, errno);
}

int ipc_receive(ipc_params_t params, void * buffer, int size){
	
	struct msg new_msg;
	int rec;
	
	new_msg.mtype = params->unique_mq_id;
	
	rec = msgrcv(params->unique_mq_id, buffer, size, 0, MSG_NOERROR);	
	
	memcpy(buffer, new_msg.info, size);
	
	return rec;
}

void ipc_close(ipc_params_t params){
	
}
