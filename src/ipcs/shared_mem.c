#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>


#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void ipc_create(ipc_params_t params){	
		
	struct shmid_ds shmbuffer;
	int segment_size = params->shm_segment_size;
	
	params->segment_id = shmget(params->unique_id, segment_size + sizeof(int),
	 IPC_CREAT | S_IRUSR | S_IWUSR);
}

void ipc_destroy(ipc_params_t params){	
	shmctl(params->segment_id, IPC_RMID, 0);
}

void ipc_open(ipc_params_t params, int action){
	/* Attach the shared memory segment. */
	params->segment_id = shmget(params->unique_id, params->shm_segment_size + sizeof(int), 0666);
	params->shared_memory_address = (char*) shmat (params->segment_id, NULL, action);
}

void ipc_close(ipc_params_t params){
	shmdt(params->shared_memory_address);

}

void ipc_send(ipc_params_t params, void * message, int size){
	
	void * new_address = params->shared_memory_address + 4;
	int key = 0;
	while(!key)
		memcpy(&key, params->shared_memory_address, sizeof(int));	

	key = params->unique_id;
	memcpy(params->shared_memory_address, &key, sizeof(int));
	memcpy(new_address, message, size);
	
}


int ipc_receive(ipc_params_t params, void * buffer, int size){
	int key;
	memcpy(&key, params->shared_memory_address, sizeof(int));
	if (!key)
		return 0; 
	void * new_address = params->shared_memory_address + 4;
	memcpy(buffer,new_address,size);
	key = 0;
	memcpy(params->shared_memory_address, &key, sizeof(int));
	return size;
}
