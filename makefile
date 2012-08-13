CC := gcc
CFLAGS := -g -lpthread

SRC1 := Processes
SRC2 := Data_Structures

all: tp1

tp1: processes.o data_structures.o server.o parser.o
	$(CC) processes.o data_structures.o server.o parser.o -o tp1

processes.o: Processes/%.c
       $(CC) $(CFLAGS) $(SRC1)/*.c

data_structures.o: Data_Structures/%.c
       $(CC) $(CFLAGS) $(SRC2)/*.c

server.o: server.c
	$(CC) $(CFLAGS) server.c

parser.o: parser.c
	$(CC) $(CFLAGS) parser.c

clean:
	rm -rf *o all
