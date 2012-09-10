#include <sys/sem.h>

#include "../../include/structs.h" 

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
	sem.sem_flg = 0;
	
	semop( semid, &sem, 1 );
}

void sem_consume(int semid, cursor_t shmem_name)
{	
	int aux = semctl( semid, shmem_name, GETVAL );
	
	semctl( semid, shmem_name, SETVAL, aux - 1 );
}
