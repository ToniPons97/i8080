CC = gcc
CFLAGS = -Wall

main: main.o i8080_disassembler.o
	$(CC) -o main main.o i8080_disassembler.o $(CFLAGS)

main.o: main.c i8080_disassembler.h
	$(CC) -c main.c $(CFLAGS)

i8080_disassembler.o: i8080_disassembler.c i8080_disassembler.h
	$(CC) -c i8080_disassembler.c $(CFLAGS)

clean:
	rm -f main *.o