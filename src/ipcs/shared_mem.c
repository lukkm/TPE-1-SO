#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "../../include/structs.h"
#include "../../include/defs.h"
#include "../../include/ipcs/ipcs.h"
#include "../../include/utils/semaphore.h"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

char * get_ipc_path(){
	char * path;
	path = calloc(11, sizeof(char));
	strcpy(path, "bin/shmem/"); 
	path[10] = 0;
	return path;
}

void ipc_create(ipc_params_t params)
{	
	int segment_size = params->shm_segment_size;
	void * shmem_address;

	params->segment_id = shmget(params->unique_id, segment_size, 
								IPC_CREAT | 0666);
}

void ipc_destroy(ipc_params_t params)
{	
	shmctl(params->segment_id, IPC_RMID, 0);
}
	
void ipc_open(ipc_params_t params, int action)
{
	params->segment_id = shmget(params->unique_id, 
						params->shm_segment_size, 0666);
	
	params->shared_memory_address = (char*) shmat (params->segment_id, 
													NULL, action);
}

void ipc_close(ipc_params_t params)
{
	shmdt(params->shared_memory_address);
}

void ipc_send(ipc_params_t params, void * message, int size)
{
	sem_consume(params->semid, params->wr_sem);
	memcpy(params->shared_memory_address, message, size);
	sem_post(params->semid, params->rd_sem);
}

int ipc_receive(ipc_params_t params, void * buffer, int size)
{	
	sem_consume(params->semid, params->rd_sem);
	memcpy(buffer, params->shared_memory_address, size);
	sem_post(params->semid, params->wr_sem);
	return size;
}

