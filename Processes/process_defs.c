#include "../structs.h"
#include "../defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>

extern process_t inc_process;
extern process_t dec_process;
extern process_t mr_process;
extern process_t ml_process;
extern process_t cz_process;
extern process_t if_process;
extern process_t endif_process;
extern process_t while_process;
extern process_t endwhile_process;

void init(){
	inc_process = calloc(1, sizeof(struct process));
        dec_process = calloc(1, sizeof(struct process));
	mr_process = calloc(1, sizeof(struct process));
	ml_process = calloc(1, sizeof(struct process));
	cz_process = calloc(1, sizeof(struct process));
        if_process = calloc(1, sizeof(struct process));
	endif_process = calloc(1, sizeof(struct process));
	while_process = calloc(1, sizeof(struct process));
	endwhile_process = calloc(1, sizeof(struct process));

	inc_process->type = INC;
	dec_process->type = DEC;
	mr_process->type = MR;
	ml_process->type = ML;
	cz_process->type = CZ;
	if_process->type = IF;
	endif_process->type = ENDIF;
	while_process->type = WHILE;
	endwhile_process->type = ENDWHILE;
}
