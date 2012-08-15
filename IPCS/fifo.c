#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "../structs.h"
#include "ipcs.h"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void ipc_create(ipc_params_t params){	
	if ( mkfifo(params->file, 0666) == -1 )
		fatal("Error mknod");
}

void ipc_open(ipc_params_t params, int action){
	int fd;
	fd = open(params->file, action);
	params->fd = fd;
}

void ipc_close(ipc_params_t params){
	close(params->fd);
}

void ipc_send(ipc_params_t params, char * message, int size){
	write(params->fd, message, size);
}


int ipc_receive(ipc_params_t params, char * buffer, int size){
	return read(params->fd, buffer, size);
}
