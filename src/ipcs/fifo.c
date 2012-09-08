#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "../../include/structs.h"
#include "../../include/ipcs/ipcs.h"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

char * get_ipc_path(){
	char * path;
	path = calloc(11, sizeof(char));
	strcpy(path, "bin/fifos/"); 
	path[10] = 0;
	return path;
}

void ipc_create(ipc_params_t params){	
	if ( mkfifo(params->file, 0666) == -1 ) {
		if (errno != EEXIST)
			fatal("Error mkfifo");
	}
}

void ipc_open(ipc_params_t params, int action){
	int fd;
	fd = open(params->file, action);
	params->fd = fd;
}

void ipc_close(ipc_params_t params){
	close(params->fd);
}

void ipc_send(ipc_params_t params, void * message, int size){
	write(params->fd, message, size);
}


int ipc_receive(ipc_params_t params, void * buffer, int size){
	return read(params->fd, buffer, size);
}

void ipc_destroy(ipc_params_t params){
	unlink(params->file);
}
