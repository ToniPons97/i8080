CC = gcc
CFLAGS = -Wall -lSDL2

main: main.o cpu.o debugger.o display.o file_handler.o
	$(CC) -o main main.o cpu.o debugger.o display.o file_handler.o $(CFLAGS)

main.o: main.c cpu.h debugger.h display.h file_handler.h
	$(CC) -c main.c $(CFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CFLAGS)

debugger.o: debugger.c debugger.h
	$(CC) -c debugger.c $(CFLAGS)

display.o: display.c display.h
	$(CC) -c display.c $(CFLAGS)

file_handler.o: file_handler.c file_handler.h
	$(CC) -c file_handler.c $(CFLAGS)
clean:
	rm -f main *.o