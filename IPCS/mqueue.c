#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/msg.h>
#include "../structs.h"
#include "ipcs.h"


#include "../parser.h"
#include "../defs.h"
#include "../Data_Structures/graph.h"
#include "../Data_Structures/stack.h"
#include "ipcs.h"

void ipc_create_msgqueue(ipc_params_t params);

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
}

/*
struct q_msg{
	
 long mtype;
 char mtext[200];
};*/

void ipc_create_msgqueue(ipc_params_t params){	
		
	if ((params->unique_mq_id = msgget((key_t)IPC_PRIVATE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) ==-1)
		perror("Error in MessageQueue Allocation");  //VER PERMISOS
	else			
	 printf("Message Queue alocada en %d \n", params->unique_mq_id);
	return;
}

void ipc_open(ipc_params_t params, int action){

	printf("Conectado a la única Message Queue ID: %d\n", params->unique_mq_id);
}

void ipc_send(ipc_params_t params, void * message, int size){

		
	if (!params->unique_mq_id)
	{
		perror("Error MessageQueue not set \n");
		exit(1);
	}
	int msg = msgsnd(params->unique_mq_id,message,size,0); //O IPC_NOWAIT , Y EL PROCESO NO SE BLOQUEA SI TIENE QUE ESPERAR
	
	printf("Enviado el mensaje %s \n", (char*) message);
	if (msg) 
		printf("Code: %d \n",msg);
}

int ipc_receive(ipc_params_t params, void * buffer, int size){
	
	int mleng;
	char* msg = malloc(1000);


        printf("Leyendo desde Queue ID: %d\n", params->unique_mq_id);
	if ((mleng = msgrcv(params->unique_mq_id,msg,size,0, MSG_NOERROR)) == -1)
	{		perror("Message Recieve Error");
			exit(1);
	}
        msg = realloc(msg,mleng);
	msg[mleng] = 0;
	printf("Recibido:   %s\n",msg);

	
	return 1;
	
}
