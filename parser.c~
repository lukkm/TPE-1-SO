#define CANT_INSTRUCTIONS 9
#define MAX_INSTRUCTION_LENGTH 100

/* Despues moverlo a un archivo con los defines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * command_list[CANT_INSTRUCTIONS];
void (* functions_list[CANT_INSTRUCTIONS]) (char *);

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

	/* Faltan hacer las funciones 

	functions_list[0] = &parse_inc
	functions_list[1] = &parse_dec
	functions_list[2] = &parse_mr
	functions_list[3] = &parse_ml
	functions_list[4] = &parse_cz
	functions_list[5] = &parse_if
	functions_list[6] = &parse_endif
	functions_list[7] = &parse_while
	functions_list[8] = &parse_endwhile

	*/

}

void select_instruction(char * instr){
	
	int i, cmd_length;

	if (instr[0] != 0){
		for(i = 0; i < CANT_INSTRUCTIONS; i++){
			cmd_length = strlen(command_list[i]);
			if (!strncmp(instr,command_list[i], cmd_length)){
				functions_list[i](instr);
				return;
			}
		}
	}

}


void parse_inc(char * instr){
	int num;
	if (sscanf(instr, "Inc(%d)", &num)){
		/* exec_inc(num); */
	}
}

void parse_dec(char * instr){
	int num;
	if (sscanf(instr, "Dec(%d)", &num)){
		/* exec_dec(num); */
	}
}

void parse_mr(char * instr){
	int num;
	if (sscanf(instr, "MR(%d)", &num)){
		/* exec_mr(num); */
	}
}

void parse_ml(char * instr){
	int num;
	if (sscanf(instr, "ML(%d)", &num)){
		/* exec_ml(num); */
	}
}

void parse_cz(char * instr){
	int num;
	if (sscanf(instr, "CZ(%d)", &num)){
		/* exec_cz(num); */
	}
}

void parse_if(char * instr){
	int num;
	char new_instr[MAX_INSTRUCTION_LENGTH];
	if (sscanf(instr, "IF(%d,%s)", &num, new_instr)){
		/* exec_if(num, new_instr); */
	}
}

void parse_endif(char * instr){
	int num;
	if (sscanf(instr, "ENDIF(%d,%s)", &num)){
		/* exec_endif(num); */
	}
}

void parse_while(char * instr){
	int num;
	char new_instr[MAX_INSTRUCTION_LENGTH];
	if (sscanf(instr, "WHILE(%d,%s)", &num, new_instr)){
		/* exec_while(num, new_instr); */
	}
}

void parse_endwhile(char * instr){
	int num;
	if (sscanf(instr, "ENDWHILE(%d,%s)", &num)){
		/* exec_endwhile(num); */
	}
}




