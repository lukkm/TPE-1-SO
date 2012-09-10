/*
 *  
 *   pre_execute
 * 	 En base a sus parametros, devuelve una estructura
 * 	 sencilla con los datos necesarios para operar el 
 * 	 proceso. Entre ellos, se encuentra el grafo del programa.
 *    
 * */
process_params_t pre_execute(status_t program, graph_t mem);
/*
 *  
 *   true_step
 * 	 setea el nodo current en verdadero,
 *   y llama al proximo paso.
 *    
 * */
void true_step(process_params_t par);
/*
 *  
 *   false_step
 * 	 setea el nodo current en falso,
 *   y llama al proximo paso
 *    
 * */
void false_step(process_params_t par);
/*
 *  
 *   conditional_step
 *   setea el nodo current en una expresion condicional
 *   y llama al proxima paso
 *    
 * */
void conditional_step(process_params_t par);
/*
 *  
 *   run_process
 *   ejecuta un nuevo thread, en base a la instruccion
 *   seleccionada, y con los parametros adecuados
 * 
 *    
 * */
void run_process(status_t c_program, void * (* execute_func) (void *));
/*
 *  
 *   get_params_from_pid
 *   devuelve una estructura que contiene la informacion para 
 *   cada ipc, y la identifica univocamente por su pid.
 *   
 *    
 * */
ipc_params_t get_params_from_pid(int pid, int type, int shm_size, int aux_semid);
/*
 *  
 *   end_process
 *   libera los procesos de memoria
 *    
 * */
void end_process();
