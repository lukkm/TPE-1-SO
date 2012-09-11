#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../../include/structs.h"
#include "../../include/data_structures/stack.h"
#include "../../include/defs.h"

#define CLEAN_LINE int i;for(i=0;i<MAX_INSTRUCTION_LENGTH;i++)line[i]=0;

char * command_list[CANT_INSTRUCTIONS];
int (* functions_list[CANT_INSTRUCTIONS]) (char *, mstack_t, instruction_t);
void set_lists(void);

int parse_inc(char * instr, mstack_t stack, instruction_t new_instr);
int parse_dec(char * instr, mstack_t stack, instruction_t new_instr);
int parse_mr(char * instr, mstack_t stack, instruction_t new_instr);
int parse_ml(char * instr, mstack_t stack, instruction_t new_instr);
int parse_cz(char * instr, mstack_t stack, instruction_t new_instr);
int parse_if(char * instr, mstack_t stack, instruction_t new_instr);
int parse_endif(char * instr, mstack_t stack, instruction_t new_instr);
int parse_while(char * instr, mstack_t stack, instruction_t new_instr);
int parse_endwhile(char * instr, mstack_t stack, instruction_t new_instr);

int parse_string(char * string, mstack_t c_stack);

extern process_t inc_process;
extern process_t dec_process;
extern process_t mr_process;
extern process_t ml_process;
extern process_t cz_process;
extern process_t if_process;
extern process_t endif_process;
extern process_t while_process;
extern process_t endwhile_process;

mstack_t parse_file(char * file_adress){
	FILE * file;
	char c;
	int i, lines = 1;
	set_lists();
	file = fopen(file_adress, "r");
	if (file == NULL){
		errno = FILE_NOT_FOUND;
		return NULL;
	}
	mstack_t stack = create_stack();
	char * line = calloc(MAX_INSTRUCTION_LENGTH, sizeof(char));
	while(!feof(file)){
		i = 0;
		while((c = fgetc(file)) != '\n' && !feof(file)){
			line[i++] = c;
		}
		line[i] = 0;
		if(*line != 0 && parse_string(line, stack) == -1){
			errno = (lines * -1) - 2;
			return NULL;
		}
		lines++;
		CLEAN_LINE;
	}
	free(line);
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

int parse_string(char * string, mstack_t c_stack){
	int i = 0;
	int count = 0, flag = 0;
	char * aux_str;
	
	while(string[i]){
		if (string[i] == '(') {
			count++;
			flag = 1;
		} else if (string[i] == ')'){
			count--;
			if (flag && !count){
				string[++i] = 0;
				aux_str = string+i+1;
				if (select_instruction(string, c_stack) == -1)
					return -1;
				string = aux_str;
				flag = 0;;
				i = -1;
			}
		} else if (!strncmp(string, "CZ", 2)){
			i+=2;
			string[i] = 0;
			aux_str = string+i+1;
			if (select_instruction(string, c_stack) == -1)
				return -1;
			string = aux_str;
			i = -1;
		}
		i++;
		
	}
	if (i)
		return select_instruction(string, c_stack);
	return 0;
}

int select_instruction(char * instr, mstack_t stack){
	
	int i, cmd_length, ret;
	instruction_t new_instr;

	if (instr[0] != 0){
		for(i = 0; i < CANT_INSTRUCTIONS; i++){
			cmd_length = strlen(command_list[i]);
			if (!strncmp(instr,command_list[i], cmd_length)){
				new_instr = calloc(1, sizeof(struct instruction));
				if (new_instr == NULL)
					return -1;
				return functions_list[i](instr, stack, new_instr);
			}
		}
	}
	return -1;
}


int parse_inc(char * instr, mstack_t stack, instruction_t new_instr){
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

int parse_dec(char * instr, mstack_t stack, instruction_t new_instr){
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

int parse_mr(char * instr, mstack_t stack, instruction_t new_instr){
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

int parse_ml(char * instr, mstack_t stack, instruction_t new_instr){
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

int parse_cz(char * instr, mstack_t stack, instruction_t new_instr){
	int num;
	if (!strcmp(instr, "CZ")){
		new_instr->instruction_type = cz_process;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_if(char * instr, mstack_t stack, instruction_t new_instr){
	int num, size, i = 0;
	char * pos;
	char * expr = calloc(1, MAX_INSTRUCTION_LENGTH);
	if (sscanf(instr, "IF(%d,", &num)){
		new_instr->instruction_type = if_process;
		new_instr->param = num;
		pos = strchr(instr, ',') + 1;
		while(pos[i] != 0){
			expr[i] = pos[i];
			i++;		
		}
		expr[strlen(expr)-1] = 0;
		new_instr->expr = expr;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_endif(char * instr, mstack_t stack, instruction_t new_instr){
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

int parse_while(char * instr, mstack_t stack, instruction_t new_instr){
	int num, size, i = 0;
	char * pos;
	char * expr = calloc(1, MAX_INSTRUCTION_LENGTH);
	if (sscanf(instr, "WHILE(%d,", &num)){
		new_instr->instruction_type = while_process;
		new_instr->param = num;
		pos = strchr(instr, ',') + 1;
		while(pos[i] != 0){
			expr[i] = pos[i];
			i++;		
		}
		expr[strlen(expr)-1] = 0;
		new_instr->expr = expr;
		if (push(stack, new_instr) == -1)
			return -1;
	}else{
		return -1;
	}
}

int parse_endwhile(char * instr, mstack_t stack, instruction_t new_instr){
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
