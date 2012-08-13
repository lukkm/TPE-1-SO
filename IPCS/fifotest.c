#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "ipcs.h"

int main(void){

	char * buf = "holaaaa";
	char buf2[200];
	int fd;
	char *name = "/tmp/fifo";
	int pid,n;

	system("rm /tmp/fifo");

	ipc_create(name);

	switch ( pid = fork() )
	{
		case -1:
			fatal("Error fork");
			break;

		case 0: /* hijo */
			fd = open(name, O_RDONLY);

			ipc_receive(fd, buf2, sizeof buf2);
			
			printf("Hijo lee del pipe: %s", buf2);
			
			printf("Hijo termina\n");

			ipc_close(fd);
			break;
		
		default: /* padre */
			fd = open(name, O_WRONLY);
			ipc_send(fd, buf, sizeof buf);
			
			printf("Padre escribe en el pipe: %s", buf);
			write(fd, buf, n);
			
			printf("Padre termina\n");
			break;
	}

	return 0;
}
