/* Para mayor informacion consultar man semop en la terminal
 *  
 *   sem_wait
 * 	 setea la operacion del semaforo en 0 (sem_op)
 *   
 *    
 * */
void sem_wait(int, cursor_t);
/*
 *  
 *   sem_post
 * 	 setea la operacion del semaforo en 1 (sem_op)
 *   
 *    
 * */
void sem_post(int, cursor_t);
/*
 *  
 *   sem_consume
 * 	 ejecuta la operacion de control de semaforos
 *   semctl. Primero hace una operacion de GETVAL, 
 *   luego finaliza con una operacion de SETVAL
 *    
 * */
void sem_consume(int, cursor_t);
