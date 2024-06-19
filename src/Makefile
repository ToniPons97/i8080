CC = gcc
CFLAGS = -Wall

main: main.o i8080_disassembler.o i8080_cpu.o
	$(CC) -o main main.o i8080_disassembler.o i8080_cpu.o $(CFLAGS)

main.o: main.c i8080_disassembler.h i8080_cpu.h
	$(CC) -c main.c $(CFLAGS)

i8080_disassembler.o: i8080_disassembler.c i8080_disassembler.h
	$(CC) -c i8080_disassembler.c $(CFLAGS)

i8080_cpu.o: i8080_cpu.c i8080_cpu.h
	$(CC) -c i8080_cpu.c $(CFLAGS)

clean:
	rm -f main *.o