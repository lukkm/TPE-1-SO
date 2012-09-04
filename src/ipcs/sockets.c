#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#define NOTSET -1

#include "../../include/structs.h"
#include "../../include/utils/parser.h"
#include "../../include/defs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_structures/stack.h"
#include "../../include/ipcs/ipcs.h"

void server_listen(void);




static int socketserv = NOTSET;
static int socketclient = NOTSET;

int main ()
{
 
	 char read_string[100];
	 int i = 0;
	 ipc_params_t server_params = calloc(1, sizeof(struct ipc_params));

 	 printf("\n\n");
	 printf("Sockets StandAlone - Program \n\n");
	 ipc_create(server_params);
	
	 //server_listen();
	 return 0;
}


//SERVER

void ipc_create(ipc_params_t params){	

	int newsockfd;
	unsigned int len;
	int listenv;
	struct sockaddr_un client; 
	socklen_t client_name_len;
        client_name_len = sizeof(struct sockaddr);
	void* buffer;
	


	//struct sockaddr_in server = {AF_INET,7000,INADDR_ANY}; //INTERNET USE CON AF_INET
	struct sockaddr_un server;  //LOCAL USE CON AF_LINUX

 	printf("Creating a socket server \n\n");	

	//params->sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	if ((params->sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket Creation Failed \n");
		exit(1);
	}
		
	
	//BINDING AF_UNIX

	server.sun_family = AF_UNIX;  /* o AF_UNIX local is declared before socket() ^ */
	strcpy(server.sun_path, "/tmp/sockets");
	unlink(server.sun_path);
	len = strlen(server.sun_path) + sizeof(server.sun_family);
	printf("Params SOCK FD: %d \n", params->sockfd);
	
	if((bind(params->sockfd, (struct sockaddr *)&server, SUN_LEN(&server)) == -1))
	{
	  perror("Bind call Failed \n");
	  exit(1);
	}

	listenv = listen(params->sockfd,5);
	
	
	if(listenv == -1)
	{
	 	perror("Listen call Failed \n");
		exit(1);
	}
        printf("Listening... %d \n",listenv);
	
	printf("Waiting for connection... %d \n",listenv);
	
     
	if ((params->client_sockfd = accept(params->sockfd, (struct sockaddr *)&client, &client_name_len)) == -1) {
            error("Error accepting connection from socket %d", params->sockfd);
      	 }
	
	ipc_receive(params, buffer, -1);
		
     
}


//CLIENT
void ipc_open(ipc_params_t params, int action){
	
	

	  if ((params->client_sockfd=socket(AF_INET, SOCK_STREAM,0)) == -1)
	  {
		perror("Client Socket Creation call Failed \n");	
		exit(1);
	  }

	  if( connect(params->client_sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
 	  {
		perror("Client Socket Connect call Failed \n");	
		exit(1);
	  }
	
	
}


void ipc_close(ipc_params_t params){
	
	printf("Cerrando Cliente \n");
	close(params->client_sockfd);
}

void ipc_closesrv(ipc_params_t params){
	
	printf("Cerrando Server \n");
	close(params->sockfd);
}

void ipc_send(ipc_params_t params, void * message, int size){
	
	/* Create the socket. */
	int socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	/* Store the server’s name in the socket address. */
	name.sun_family = AF_LOCAL;
	strcpy(name.sun_path, socket_name);
	/* Connect the socket. */
	connect(socket_fd, &name, SUN_LEN (&name));
	/* Write the text on the command line to the socket. */
	write_text(socket_fd, message);
	close (socket_fd);
	return 0;

	
}

void write_text (int socket_fd, const char* text)
{
	/* Write the number of bytes in the string, including
	NUL-termination. */
	int length = strlen (text) + 1;
	write(socket_fd, &length, sizeof(length));
	/* Write the string. */
	
	write(socket_fd, text, length);
	printf("Enviado: %s\n", (char*) text);	
	return;
}



int ipc_receive(ipc_params_t params, void * buffer, int size){
	int length;
	char* text;
	
	if (read(params->client_sockfd, &length, sizeof (length)) == 0)
		return 0;
	/* Allocate a buffer to hold the text. */
	text = (char*) malloc(length);
	/* Read the text itself, and print it. */
	read(client_socket, text, length);
	
	printf("Recibiendo: %s\n", (char*) text);
	free(text);
}




