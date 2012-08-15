/* Despues moverlo a un archivo con los defines */
/*Alan*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "Data_Structures/stack.h"
#include "defs.h"

/* Include "processes.h" */

char * command_list[CANT_INSTRUCTIONS];
int (* functions_list[CANT_INSTRUCTIONS]) (char *, stack_t, instruction_t);
void set_lists(void);

int parse_inc(char * instr, stack_t stack, instruction_t new_instr);
int parse_dec(char * instr, stack_t stack, instruction_t new_instr);
int parse_mr(char * instr, stack_t stack, instruction_t new_instr);
int parse_ml(char * instr, stack_t stack, instruction_t new_instr);
int parse_cz(char * instr, stack_t stack, instruction_t new_instr);
int parse_if(char * instr, stack_t stack, instruction_t new_instr);
int parse_endif(char * instr, stack_t stack, instruction_t new_instr);
int parse_while(char * instr, stack_t stack, instruction_t new_instr);
int parse_endwhile(char * instr, stack_t stack, instruction_t new_instr);

extern process_t inc_process;
extern process_t dec_process;
extern process_t mr_process;
extern process_t ml_process;
extern process_t cz_process;
extern process_t if_process;
extern process_t endif_process;
extern process_t while_process;
extern process_t endwhile_process;

stack_t parse_file(char * file_adress){
	FILE * file;
	set_lists();
	file = fopen(file_adress, "r");
	if (file == NULL)
		return NULL;
	stack_t stack = create_stack();
	char line[MAX_INSTRUCTION_LENGTH];	
	while(!feof(file)){
		fscanf(file, "%s", line);
		if (select_instruction(line, stack) == -1)
			return NULL;
	}
	return stack;
}

void set_lists(){
	command_list[0] = "Inc";
	command_list[1] = "Dec";
	command_list[2] = "MR";
	command_list[3] = "ML";
	command_list[4] = "CZ";
	command_list[5] = "IF";
	command_list[6] = "ENDIF";
	command_list[7] = "WHILE";
	command_list[8] = "ENDWHILE";

	functions_list[0] = &parse_inc;
	functions_list[1] = &parse_dec;
	functions_list[2] = &parse_mr;
	functions_list[3] = &parse_ml;
	functions_list[4] = &parse_cz;
	functions_list[5] = &parse_if;
	functions_list[6] = &parse_endif;
	functions_list[7] = &parse_while;
	functions_list[8] = &parse_endwhile;
}

int select_instruction(char * instr, stack_t stack){
	
	int i, cmd_length;

	if (instr[0] != 0){
		for(i = 0; i < CANT_INSTRUCTIONS; i++){
			cmd_length = strlen(command_list[i]);
			if (!strncmp(instr,command_list[i], cmd_length)){
				instruction_t new_instr = calloc(1, sizeof(struct instruction));
				if (new_instr == NULL)
					return -1;
				return functions_list[i](instr, stack, new_instr);
			}
		}
	}
	return -1;
}


int parse_inc(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "Inc(%d)", &num)){
		new_instr->instruction_type = inc_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_dec(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "Dec(%d)", &num)){
		new_instr->instruction_type = dec_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_mr(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "MR(%d)", &num)){
		new_instr->instruction_type = mr_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_ml(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "ML(%d)", &num)){
		new_instr->instruction_type = ml_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_cz(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "CZ")){
		new_instr->instruction_type = cz_process;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_if(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	char expr[MAX_INSTRUCTION_LENGTH];
	if (sscanf(instr, "IF(%d,%s)", &num, expr) == 2){
		new_instr->instruction_type = if_process;
		new_instr->param = num;
		new_instr->expr = expr;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_endif(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "ENDIF(%d)", &num)){
		new_instr->instruction_type = endif_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_while(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	char expr[MAX_INSTRUCTION_LENGTH];
	if (sscanf(instr, "WHILE(%d,%s)", &num, expr) == 2){
		new_instr->instruction_type = while_process;
		new_instr->param = num;
		new_instr->expr = expr;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_endwhile(char * instr, stack_t stack, instruction_t new_instr){
	int num;
	if (sscanf(instr, "ENDWHILE(%d)", &num)){
		new_instr->instruction_type = endwhile_process;
		new_instr->param = num;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}




