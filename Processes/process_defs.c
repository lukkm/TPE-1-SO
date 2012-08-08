#include "../structs.c"
#include "../defs.h"

process_t inc_process;
process_t dec_process;
process_t mr_process;
process_t ml_process;
process_t cz_process;
process_t if_process;
process_t endif_process;
process_t while_process;
process_t endwhile_process;

void init(){
	inc_process = calloc(sizeof(struct process));
        dec_process = calloc(sizeof(struct process));
	mr_process = calloc(sizeof(struct process));
	ml_process = calloc(sizeof(struct process));
	cz_process = calloc(sizeof(struct process));
        if_process = calloc(sizeof(struct process));
	endif_process = calloc(sizeof(struct process));
	while_process = calloc(sizeof(struct process));
	endwhile_process = calloc(sizeof(struct process));

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
