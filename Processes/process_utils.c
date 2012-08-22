#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "../structs.h"
#include "../IPCS/ipcs.h"

process_params_t pre_execute(status_t program, int param)
{
  	process_params_t ret = (process_params_t)calloc(1, sizeof(struct process_params));
	ret->param = param;
	ret->c_status = program;
	return ret;
}

void call_next_process(status c_program, ipc_params_t params){
	ipc_open(params, O_WRONLY);
	ipc_send(params, &c_program, sizeof(struct status));
	ipc_close(params);
	return;
}
