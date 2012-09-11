/*
 * build_graph 
 * En base a un stack ya armado de instrucciones
 * construye el grafo del codigo
 * 
 * */

graph_t build_graph(mstack_t instructions);

int get_graph_size(graph_t);

/*
 * 
 * free_graph
 * Libera toda la memoria ocupada por el grafo que recibe como parametro.
 * 
 * */

void free_graph(graph_t);




