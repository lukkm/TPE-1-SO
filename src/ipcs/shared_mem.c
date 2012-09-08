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

void sem_consume(int semid, cursor_t shmem_name);
void sem_post(int semid, cursor_t shmem_name);
void sem_wait(int semid, cursor_t shmem_name);

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void ipc_create(ipc_params_t params)
{	
	//struct shmid_ds shmbuffer;
	int segment_size = params->shm_segment_size;
	void * shmem_address;

	params->segment_id = shmget(params->unique_id, segment_size, 
								IPC_CREAT | 0666);
	//shmem_address = shmat(params->segment_id, NULL, 0666);
	//shmdt(shmem_address);
}

void ipc_destroy(ipc_params_t params)
{	
	shmctl(params->segment_id, IPC_RMID, 0);
}
	
void ipc_open(ipc_params_t params, int action)
{
	/* Attach the shared memory segment. */
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
	//printf("%d\n", ((client_header_t)message)->client_id);
	//printf("%d\n", ((client_header_t)message)->program_size);
	
	printf("%d %d\n", params->shmem_name, semctl(params->semid, params->shmem_name, GETVAL));

	//sem_wait(params->semid, params->shmem_name);

	sem_consume(params->semid, params->shmem_name);
	
	memcpy(params->shared_memory_address, message, size);
	
	
	//printf("%d\n", ((client_header_t)params->shared_memory_address)->client_id);
	//printf("%d\n", ((client_header_t)params->shared_memory_address)->program_size);
	
	//printf("%d %d %d\n", semctl(params->semid, params->shmem_name, GETVAL), params->semid, params->shmem_name);
}

int ipc_receive(ipc_params_t params, void * buffer, int size)
{	
	//printf("%d %d\n", params->shmem_name, semctl(params->semid, params->shmem_name, GETVAL));
	
	if ( semctl(params->semid, params->shmem_name, GETVAL) >= 0 )
		return 0;
	//printf("%d %d %d\n", semctl(params->semid, params->shmem_name, GETVAL), params->semid, params->shmem_name);
		
	memcpy(buffer, params->shared_memory_address, size);
	
	sem_post(params->semid, params->shmem_name);
	
	//printf("%d\n", ((client_header_t)params->shared_memory_address)->client_id);
	//printf("%d\n", ((client_header_t)params->shared_memory_address)->program_size);
	
	//sem_consume(params->semid, params->shmem_name);
	
	//printf("%d %d %d\n", semctl(params->semid, params->shmem_name, GETVAL), params->semid, params->shmem_name);
	
	return size;
}

void sem_wait(int semid, cursor_t shmem_name) 
{
	struct sembuf sem;
	
	sem.sem_num = shmem_name;
	sem.sem_op = 0;
	sem.sem_flg = 0;
	
	semop( semid, &sem, 1 );
}

void sem_post(int semid, cursor_t shmem_name)
{
	struct sembuf sem;
	
	sem.sem_num = shmem_name;
	sem.sem_op = 1;
	sem.sem_flg = SEM_UNDO;
	
	semop( semid, &sem, 1 );
}

void sem_consume(int semid, cursor_t shmem_name)
{
	struct sembuf sem;
	
	sem.sem_num = shmem_name;
	sem.sem_op = -1;
	sem.sem_flg = SEM_UNDO;
	
	semop( semid, &sem, 1 );
}
