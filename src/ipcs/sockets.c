#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>


#include "../../include/structs.h"
#include "../../include/utils/parser.h"
#include "../../include/defs.h"
#include "../../include/data_structures/graph.h"
#include "../../include/data_structures/stack.h"
#include "../../include/ipcs/ipcs.h"

void server_listen(void);


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


	//int sockfd;
	int newsockfd;
	unsigned int len;
	int listenv;

	struct sockaddr_in server = {AF_INET,7000,INADDR_ANY}; //INTERNET USE CON AF_INET
	struct sockaddr_un local, remote;  //LOCAL USE CON AF_LINUX

	/*struct sockaddr_un {
	    unsigned short sun_family; 
	    char sun_path[108];
	}*/

 	printf("Creating a socket server \n\n");	


	if ((params->sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket Creation Failed \n");
		exit(1);
	}
	
	//BINDING AF_UNIX

	local.sun_family = AF_UNIX;  /* local is declared before socket() ^ */
	
	strcpy(local.sun_path, "/tmp");
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	printf("Params SOCK FD: %d \n", params->sockfd);
	if((bind(params->sockfd, (struct sockaddr *)&local, len) == -1))
	{
		printf("%d\n", errno);
		perror("Bind call Failed \n");
		exit(1);
	}



	//BINDING AF_INET
	/*
	if( (bind(params->sockfd,(struct sockaddr*)&server, sizeof(struct sockaddr_in))) == -1)
	{
	 	perror("Bind call Failed \n");
		exit(1);
	}*/

	listenv = listen(params->sockfd,5);
	printf("Listen value: %d \n",listenv);
	
	if( listenv == -1)
	{
	 	perror("Listen call Failed \n");
		exit(1);
	}
	len = sizeof(remote);
	newsockfd = accept(params->sockfd, (struct sockaddr *)&remote, &len);
	printf("New socket File Descriptor: %d \n\n",newsockfd);
	 if(newsockfd == -1)
		{		
		 	perror("Accept Connection call Failed \n");
		}
	printf("New socket File Descriptor: %d \n\n",newsockfd);
	
	
}


//CLIENT
void ipc_open(ipc_params_t params, int action){
	

	//  int client_sockfd;
	struct sockaddr_in server = {AF_INET,7000};
	server.sin_addr.s_addr = inet_addr("206.45.10.2");
	
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

void ipc_destroy(ipc_params_t params){
	


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
	

	printf("Cliente- Enviado: %s\n", (char*) message);
	send(params->client_sockfd, (char*) message, size,0);
	return;

	
}

void ipc_sendsrv(ipc_params_t params, void * message, int size){
	

	printf("Server- Enviado: %s\n", (char*) message);
	send(params->sockfd, (char*) message, size,0);
	return;

	
}

int ipc_receive(ipc_params_t params, void * buffer, int size){
	recv(params->client_sockfd, (char*) buffer, size,0);
	printf("Cliente- Recibiendo: %s\n", (char*) buffer);

}

int ipc_receivesrv(ipc_params_t params, void * buffer, int size){
	recv(params->sockfd, (char*) buffer, size,0);
	printf("Server- Recibiendo: %s\n", (char*) buffer);

}

void write_text (int socket_fd, const char* text)
{

	/* Write the number of bytes in the string, including
	NUL-termination. */
	int length = strlen(text) + 1;
	write(socket_fd, &length, sizeof (length));
	
	/* Write the string. */
	write(socket_fd, text, length);
	return;
}


void server_listen(void)
{
    unsigned int s, s2, len;
    struct sockaddr_un local, remote;
    char code[1000];
	int deleteme = 0;
    //env_t *ans;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("unable to create socket");
    }
    printf("%d",s);
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, "../tmp/socket");
    unlink(local.sun_path);

    len = sizeof(struct sockaddr_un);

     deleteme =  bind(s, (struct sockaddr *)&local, (socklen_t) len);
     printf("%d",deleteme);
     deleteme = listen(s, 5);
     printf("%d",deleteme);

    while (1) {
        len = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &len)) == -1) {
            error("unable to accept socket connection");
        }

        switch (fork()) {
            case -1:
                error("unable to fork process");
                break;
            case 0:
             //   ans = env_init();
                while ((len = recv(s2, code, sizeof(code), 0)) > 0) {
                  //  env_run(ans, code);
                   // send(s2, ans, sizeof(*ans), 0);
                }
               // free(ans);

                close(s2);
                break;
            default:
                break;
        }
    }

    wait(NULL);
}
