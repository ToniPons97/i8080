CC = gcc
CFLAGS = -Wall -lSDL2

main: main.o cpu.o debugger.o renderer.o
	$(CC) -o main main.o cpu.o debugger.o renderer.o $(CFLAGS)

main.o: main.c cpu.h debugger.h renderer.h
	$(CC) -c main.c $(CFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CFLAGS)

debugger.o: debugger.c debugger.h
	$(CC) -c debugger.c $(CFLAGS)

renderer.o: renderer.c renderer.h
	$(CC) -c renderer.c $(CFLAGS)

clean:
	rm -f main *.o