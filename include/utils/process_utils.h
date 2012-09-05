process_params_t pre_execute(status_t program, graph_t mem);
void true_step(process_params_t par);
void false_step(process_params_t par);
void conditional_step(process_params_t par);
void run_process(status_t c_program, void * (* execute_func) (void *));
ipc_params_t get_params_from_pid(int pid, int type, int shm_size, int aux_semid);
void end_process();
