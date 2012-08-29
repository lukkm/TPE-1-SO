void ipc_create(ipc_params_t params);
void ipc_open(ipc_params_t params, int action);
void ipc_close(ipc_params_t params);
void ipc_send(ipc_params_t params, void * message, int size);
int ipc_receive(ipc_params_t params, void * buffer, int size);
void ipc_destroy(ipc_params_t params);
