all:
	rm -r -f bin
	mkdir bin
	mkdir bin/fifos
	mkdir bin/mqueue
	mkdir bin/shmem
	mkdir bin/sockets

	gcc -g -o bin/fifos/server src/server.c src/utils/parser.c src/data_structures/graph.c src/data_structures/stack.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c src/utils/shmem_graph.c -lpthread
	gcc -g -o bin/fifos/inc src/processes/inc_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/dec src/processes/dec_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/mr src/processes/mr_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/ml src/processes/ml_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/cz src/processes/cz_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/if src/processes/if_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/endif src/processes/endif_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/while src/processes/while_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/fifos/endwhile src/processes/endwhile_process.c src/ipcs/fifo.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread

	gcc -g -o bin/fifos/client src/client.c src/ipcs/fifo.c src/utils/process_utils.c src/utils/ipcs_protocol.c -lpthread

	gcc -g -o bin/mqueue/server src/server.c src/utils/parser.c src/data_structures/graph.c src/data_structures/stack.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c src/utils/shmem_graph.c -lpthread
	gcc -g -o bin/mqueue/inc src/processes/inc_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/dec src/processes/dec_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/mr src/processes/mr_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/ml src/processes/ml_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/cz src/processes/cz_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/if src/processes/if_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/endif src/processes/endif_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/while src/processes/while_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/mqueue/endwhile src/processes/endwhile_process.c src/ipcs/mqueue.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread

	gcc -g -o bin/mqueue/client src/client.c src/ipcs/mqueue.c src/utils/process_utils.c src/utils/ipcs_protocol.c -lpthread
	
	gcc -g -o bin/shmem/server src/server.c src/utils/parser.c src/data_structures/graph.c src/data_structures/stack.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c src/utils/shmem_graph.c -lpthread
	gcc -g -o bin/shmem/inc src/processes/inc_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/dec src/processes/dec_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/mr src/processes/mr_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/ml src/processes/ml_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/cz src/processes/cz_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/if src/processes/if_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/endif src/processes/endif_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/while src/processes/while_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/shmem/endwhile src/processes/endwhile_process.c src/ipcs/shared_mem.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread

	gcc -g -o bin/shmem/client src/client.c src/ipcs/shared_mem.c src/utils/process_utils.c src/utils/ipcs_protocol.c -lpthread
	
	gcc -g -o bin/sockets/server src/server.c src/utils/parser.c src/data_structures/graph.c src/data_structures/stack.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c src/utils/shmem_graph.c -lpthread
	gcc -g -o bin/sockets/inc src/processes/inc_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/dec src/processes/dec_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/mr src/processes/mr_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/ml src/processes/ml_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/cz src/processes/cz_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/if src/processes/if_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/endif src/processes/endif_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/while src/processes/while_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread
	gcc -g -o bin/sockets/endwhile src/processes/endwhile_process.c src/ipcs/sockets.c src/utils/ipcs_protocol.c src/utils/process_utils.c -lpthread

	gcc -g -o bin/sockets/client src/client.c src/ipcs/sockets.c src/utils/process_utils.c src/utils/ipcs_protocol.c -lpthread
