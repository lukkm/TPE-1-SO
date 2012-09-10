/*
 * 
 * Crea un nuevo IPC con los parametros recibidos
 * 
 * */

void ipc_create(ipc_params_t params);

/*
 * 
 * Abre el IPC definido en los parametros
 * 
 * */
 
void ipc_open(ipc_params_t params, int action);

/*
 * 
 * Cierra el IPC definido en los parametros
 * 
 * */
 
void ipc_close(ipc_params_t params);

/*
 * 
 * Manda un mensaje definido en la direccion de memoria apuntada por 
 * message, a la IPC definida por params.
 * 
 * */
 
void ipc_send(ipc_params_t params, void * message, int size);

/*
 * 
 * Recibe un mensaje de la IPC definida por params almacenandolo en 
 * la direccion de memoria apuntada por buffer.
 * 
 * */
 
int ipc_receive(ipc_params_t params, void * buffer, int size);

/*
 * 
 * Destruye el IPC definido en los parametros
 * 
 * */
 
void ipc_destroy(ipc_params_t params);

/*
 * 
 * Devuelve el nombre de la carpeta donde se encuentran almacenados 
 * los archivos binarios creados para la ipc.
 * 
 * */
 
char * get_ipc_path(void);
