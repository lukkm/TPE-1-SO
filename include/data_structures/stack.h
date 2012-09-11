/*
 * create_stack 
 * Aloca inicialmente el espacio necesario para la
 * estructura stack
 * 
 * */
mstack_t create_stack();
/*
 *  
 * push
 * Representa la tipica operacion de pila 
 * ubica el elemento node en el tope de la pila
 *    
 * */
int push(mstack_t stack, void * node);
/*
 *  
 * pop
 * Representa la tipica operacion de pila 
 * remover el elemento en el tope de la pila
 * y devolverlo
 *    
 * */
stack_node pop(mstack_t stack);
/*
 *  
 * is_empty
 * Devuelve un valor relacionado con la
 * existencia de valores en la pila, o si
 * la misma esta vacia
 *    
 * */
int is_empty(mstack_t stack);

/*
 * 
 * Libera el espacio reservado para el stack asi como para todos sus nodos internos
 * 
 * */

void free_stack(mstack_t c_stack);

