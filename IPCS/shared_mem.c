#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/stat.h>


#include "../structs.h"
#include "ipcs.h"


#include "../parser.h"
#include "../defs.h"
#include "../Data_Structures/graph.h"
#include "../Data_Structures/stack.h"
#include "ipcs.h"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

int main(void)
{

		char read_string[100];
		int i = 0;
		ipc_params_t server_params = calloc(1, sizeof(struct ipc_params));
		printf("Shared Memory StandAlone - Program \n");
		ipc_create(server_params);
		ipc_open(server_params, 0);
		//server_params->msg = "Hola, que tal \0";
		char * msg = "Hola";
		ipc_send(server_params,msg,strlen(msg)*sizeof(char));
		while(1){
		 if(ipc_receive(server_params, read_string, 100) > 0){
			printf("%s \n", read_string);
		   }
		 	sleep(1);
			
					
				
			
		  }
}

void ipc_create(ipc_params_t params){	
		
	struct shmid_ds shmbuffer;
	int segment_size = params->shm_segment_size;
	const int shared_segment_size = 0x6400; //TODO VER TAMAÑO.
	
	params->segment_id = shmget(IPC_PRIVATE, shared_segment_size,
	 IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	printf("Segmento Nro %d alocado \n", params->segment_id);
	printf("Tamaño %d bytes \n", shared_segment_size);
}

void ipc_destroy(ipc_params_t params){	
	shmctl(params->segment_id, IPC_RMID, 0);
	printf("Segmento %d desalocado \n", params->segment_id);
}

void ipc_open(ipc_params_t params, int action){
	/* Attach the shared memory segment. */
	params->shared_memory_address = (char*) shmat (params->segment_id, 0, 0);
	printf("Conectado a la porcion de memoria: %p\n", params->shared_memory_address);
}

void ipc_close(ipc_params_t params){
	shmdt(params->shared_memory_address);
	printf("Desconectado a la porcion de memoria: %p\n", params->shared_memory_address);

}

void ipc_send(ipc_params_t params, char * message, int size){

	printf("Enviado:  %s" , message);
	
	sprintf(params->shared_memory_address, message);
	printf("\n");
	
}


int ipc_receive(ipc_params_t params, char * buffer, int size){

	memcpy(buffer,params->shared_memory_address,size);
	printf("Recibido:   %s\n", params->shared_memory_address);
	return 1;
	//return read(params->fd, buffer, size);
}
