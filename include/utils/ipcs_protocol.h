/*
 *  
 * free_params
 * Libera de memoria los parametros utilizados
 * por las ipcs
 *    
 * */
void free_params(void);
/*
 *  
 * init_processes
 * Inicializa todas las variables necesarias
 * para el manejo de todos los procesos. 
 * Inicialmente las variables de las IPCS,
 * y luego genera un vector que contiene
 * punteros a funcion a cada proceso,
 * en base a las instrucciones del lenguaje
 *    
 * */
void init_processes(int);
