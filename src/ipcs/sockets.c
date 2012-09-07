#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#define READ_LENGTH 5

#include "../../include/structs.h"
#include "../../include/utils/parser.h"
#include "../../include/defs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_structures/stack.h"
#include "../../include/ipcs/ipcs.h"

void server_listen(void);

struct sockaddr_un create_struct_sockaddr(ipc_params_t params){
	struct sockaddr_un new_struct;
	
	new_struct.sun_family = AF_UNIX;
	sprintf(new_struct.sun_path, "%s", params->file);
	
	return new_struct;
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void ipc_create(ipc_params_t params){	

	/*unsigned int len;
	int bind_value, listenv	;
	struct sockaddr_un socket_params;

 	printf("\nCreating a socket server \n\n");	
	if ((params->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("Socket Creation Failed \n");
		exit(1);
	}

	printf("errno: %d\n", errno);
*/
	/*socket_params = create_struct_sockaddr(params);
	
	unlink(socket_params.sun_path);
	
	printf("Sun Path: %s\n", socket_params.sun_path);
	
	len = strlen(socket_params.sun_path) + sizeof(socket_params.sun_family);
	
	printf("Params SOCK FD: %d \n", params->sockfd);
	printf("errno: %d\n", errno);

	bind_value = bind(params->sockfd, (struct sockaddr *)&socket_params, len);
	
	if(bind_value == -1){
	  perror("Bind call Failed \n");
	  exit(1);
	}*/
	
		  
	/*listenv = listen(params->sockfd, 5);

	if(listenv == -1){
	  perror("Listen call Failed \n");
	  exit(1);
	}
	printf("Listening... %d \n",listenv);
	printf("errno: %d\n", errno);
	printf("Waiting for connection... %d \n",listenv);
	
	if ((params->client_sockfd = accept(params->sockfd, (struct sockaddr *)&socket_params, &len)) == -1) {
		error("Error accepting connection from socket %d", params->sockfd);
    }*/
}


void ipc_open(ipc_params_t params, int action){
	
	  struct sockaddr_un socket_params;
	  socklen_t size = sizeof(struct sockaddr_un);
	  int len, bind_value;
	  char * old_file;
	  char * ct_read = "read";
		  
	  if(params->sockfd == -1){
		  
		  if ((params->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
			perror("Socket Creation Failed \n");
			exit(1);
			}
			
		if(!params->socklistener){
			socket_params = create_struct_sockaddr(params);

			unlink(socket_params.sun_path);
			
			printf("Sun Path: %s\n", socket_params.sun_path);
			
			len = strlen(socket_params.sun_path) + sizeof(socket_params.sun_family);
			
			printf("Params SOCK FD: %d \n", params->sockfd);
			printf("errno: %d\n", errno);

			bind_value = bind(params->sockfd, (struct sockaddr *)&socket_params, len);
			
			if(bind_value == -1){
			  perror("Bind call Failed \n");
			  exit(1);
			}
			
			params->socklistener = 1;
		}
		
		/*socket_params = create_struct_sockaddr(params);
		
		if( connect(params->sockfd, (struct sockaddr*)&socket_params, size) == -1){
		   printf("Errno client: %d\n", errno);
		   perror("Client Socket Connect call Failed \n");	
		   exit(1);
	   }*/
		
		/*
		old_file = params->file
		total_length = strlen(old_file) + READ_LENGTH;
		params->file = calloc(total_length);
		strcpy(params->file, old_file);
		strcat(params->file, ct_read);
		params->file[total_length - 1] = 0;
		
		
		socket_params = create_struct_sockaddr(params);
		
		
		bind_value = bind(params->sockfd, (struct sockaddr *)&socket_params, len);
	
		if(bind_value == -1){
		  perror("Bind call Failed \n");
		  exit(1);
		}
		
		printf("Bind value: %d\n", bind_value);	
			  
		listenv = listen(params->sockfd, 5);

		if(listenv == -1){
		  perror("Listen call Failed \n");
		  exit(1);
		}
		printf("Listening... %d \n",listenv);
		printf("errno: %d\n", errno);
		printf("Waiting for connection... %d \n",listenv);
		
		if ((params->client_sockfd = accept(params->sockfd, (struct sockaddr *)&socket_params, &len)) == -1) {
			error("Error accepting connection from socket %d", params->sockfd);
		}
		*/	
		
	  }	

	  
}


void ipc_close(ipc_params_t params){
	
	printf("Cerrando IPC \n");
	//close(params->client_sockfd);
	//close(params->sockfd);
}

void ipc_closesrv(ipc_params_t params){
	
	printf("Cerrando Server \n");
	close(params->sockfd);
	unlink(params->file);
}

void ipc_send(ipc_params_t params, void * message, int size){
	
	/* Create the socket. */
	//int socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	/* Store the server’s name in the socket address. */
	//name.sun_family = AF_LOCAL;
	//strcpy(name.sun_path, socket_name);
	/* Connect the socket. */
	//connect(socket_fd, &name, SUN_LEN (&name));
	/* Write the text on the command line to the socket. */
	//write_text(socket_fd, message);
	//close (socket_fd);
	//return 0;
	/*if (params->client_sockfd != -1)
		write(params->client_sockfd, message, size);
	else
		write(params->sockfd, message, size);
	*/
	struct sockaddr_un socket_params;
	int len;
	socket_params = create_struct_sockaddr(params);
	len = sizeof(struct sockaddr_un);
	int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	printf("params->sockfd = %d, sun path = %s\n", params->sockfd, socket_params.sun_path);
	sendto(sockfd, message, size, 0, (struct sockaddr *) &socket_params, len);
	printf("Errno after sending: %d\n", errno);
	close(sockfd);
}

void write_text (int socket_fd, const char* text)
{
	/* Write the number of bytes in the string, including
	NUL-termination. */
	//int length = strlen (text) + 1;
	//write(socket_fd, &length, sizeof(length));
	/* Write the string. */
	
	//write(socket_fd, text, length);
	//printf("Enviado: %s\n", (char*) text);	
	//return;
}



int ipc_receive(ipc_params_t params, void * buffer, int size){
	/*int length;
	char* text;
	
	if (read(params->client_sockfd, &length, sizeof (length)) == 0)
		return 0;
	text = (char*) malloc(length);
	read(client_socket, text, length);
	
	printf("Recibiendo: %s\n", (char*) text);
	free(text);*/
	struct sockaddr_un socket_params;
	int len;
	socket_params = create_struct_sockaddr(params);
	len = sizeof(struct sockaddr_un);
	printf("sockfd = %d \n", params->sockfd);
	return recvfrom(params->sockfd, buffer, size, 0, (struct sockaddr *)&socket_params, &len);
}

void ipc_destroy(ipc_params_t params){

}



