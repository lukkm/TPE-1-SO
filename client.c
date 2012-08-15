#include <stdlib.h>
#include <fcntl.h>

#include "structs.h"
#include "IPCS/ipcs.h"



int main(void){

	ipc_params_t client_params = calloc(1, sizeof(struct ipc_params));
		
	client_params->file = "/tmp/server";

	ipc_open(client_params, O_WRONLY);

	ipc_send(client_params, "hola", 4);

	return 0;
}
