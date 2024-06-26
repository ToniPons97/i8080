CC = gcc
CFLAGS = -Wall

main: main.o cpu.o debugger.o
	$(CC) -o main main.o cpu.o debugger.o $(CFLAGS)

main.o: main.c cpu.h debugger.h
	$(CC) -c main.c $(CFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CFLAGS)

debugger.o: debugger.c debugger.h
	$(CC) -c debugger.c $(CFLAGS)

clean:
	rm -f main *.o