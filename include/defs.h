/*
 * 
 * Parametros generales del programa
 * 
 * */

#define CANT_INSTRUCTIONS 9
#define MAX_INSTRUCTION_LENGTH 100
#define MEM_SIZE 1000
#define MAX_IPC_SIZE 8190
#define MAX_PROGRAM_LENGTH 1000

#define PATH_LENGTH 10
#define TMP_LENGTH 5

#define PRE_HEADER 6
#define PROGRAM_STRING 7
#define PROGRAM_STATUS 8

#define FALSE 0
#define TRUE 1

#define SVR_RCV_RD 0
#define SVR_RCV_WR 1
#define SVR_PAR_RD 2
#define SVR_PAR_WR 3


/*
 * Valores arbitrarios definidos para cada instruccion/proceso
 * 
 * */
#define INC 0
#define DEC 1
#define MR 2
#define ML 3
#define CZ 4
#define IF 5
#define ENDIF 6
#define WHILE 7
#define ENDWHILE 8
#define MAXPRIOR 10
#define QPERM 0660

#define SEMSET_SIZE 24

/*
 * 
 * Codigos de error
 * 
 * */

#define FILE_NOT_FOUND -1
#define UNABLE_TO_BUILD_GRAPH -2
