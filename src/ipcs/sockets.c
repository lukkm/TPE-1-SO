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
			
			len = strlen(socket_params.sun_path) + sizeof(socket_params.sun_family);

			bind_value = bind(params->sockfd, (struct sockaddr *)&socket_params, len);
			
			if(bind_value == -1){
			  perror("Bind call Failed \n");
			  exit(1);
			}
			
			params->socklistener = 1;
		}
		
	  }	

	  
}


void ipc_close(ipc_params_t params){
	//close(params->client_sockfd);
	//close(params->sockfd);
}

void ipc_closesrv(ipc_params_t params){
	
	printf("Cerrando Server \n");
	close(params->sockfd);
	unlink(params->file);
}

void ipc_send(ipc_params_t params, void * message, int size){
	
	struct sockaddr_un socket_params;
	int len;
	socket_params = create_struct_sockaddr(params);
	len = sizeof(struct sockaddr_un);
	int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	sendto(sockfd, message, size, 0, (struct sockaddr *) &socket_params, len);
	close(sockfd);
}

int ipc_receive(ipc_params_t params, void * buffer, int size){

	struct sockaddr_un socket_params;
	int len;
	socket_params = create_struct_sockaddr(params);
	len = sizeof(struct sockaddr_un);
	return recvfrom(params->sockfd, buffer, size, 0, (struct sockaddr *)&socket_params, &len);
	
}

void ipc_destroy(ipc_params_t params){

}



