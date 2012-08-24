#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../include/structs.h"
#include "../../include/parser.h"
#include "../../include/defs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_ztructures/stack.h"
#include "../../include/ipcs/ipcs.h"



int ipc_receive(ipc_params_t params, void * buffer, int size)
{

 int length;
 char* text;

/* First, read the length of the text message from the socket.
read returns zero, the client closed the connection. */
 
if (read(params->socket_fd, &length, sizeof (length)) == 0)
   {
	printf("Connection closed \n");
	return 0;
   }
	 
 /* Allocate a buffer */
    text = (char*) malloc (length);
  
 /* Read the text itself, and print it. */
    read(params->socket_fd, text, length);
    printf (“%s\n”, text);
    memcpy(buffer,text,sizeof(char)*length+1);
    free (text);
 
   return 1;

}

int main (int argc, char* const argv[])
{
 
 const char* const socket_name = argv[1];

 

int client_sent_quit_message;


/* Repeatedly accept connections, spinning off one server() to deal
with each client. Continue until a client sends a “quit” message. */
do {
	struct sockaddr_un client_name;
	socklen_t client_name_len;
	int client_socket_fd;

	/* Accept a connection. */
	client_socket_fd = accept (socket_fd, &client_name, &client_name_len);
	/* Handle the connection. */
	client_sent_quit_message = ipc_receive(client_socket_fd);
	/* Close our end of the connection. */
	close (client_socket_fd);
   }
while (!client_sent_quit_message);

ipc_close(
unlink (socket_name);
return 0;
}


void ipc_create(ipc_params_t params){	
		
	/* Create the socket. */
         params->socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
	 printf("Socket creado en puerto %d \n", params->socket_fd);
	return;
}

void ipc_open(ipc_params_t params, int action){
	
	struct sockaddr_un name;
	/* Indicate that this is a server. */
	name.sun_family = AF_LOCAL;
	strcpy(name.sun_path, socket_name);
	bind(socket_fd, &name, SUN_LEN (&name));
	
	/* Listen for connections. */
        listen(socket_fd, 5);
	printf("Escuchando del puerto %d \n", params->socket_fd);
	return;
}

void ipc_destroy(ipc_params_t params){
	
	/* Remove the socket file. */
     close (params->socket_fd);

}

void ipc_close(ipc_params_t params){


}

