CC = gcc
CFLAGS = -Wall -lSDL2

main: main.o cpu.o debugger.o display.o file_handler.o keyboard_state.o si_machine_io.o
	$(CC) -o main main.o cpu.o debugger.o display.o file_handler.o keyboard_state.o si_machine_io.o $(CFLAGS)

main.o: main.c cpu.h debugger.h display.h file_handler.h keyboard_state.h si_machine_io.h
	$(CC) -c main.c $(CFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CFLAGS)

debugger.o: debugger.c debugger.h
	$(CC) -c debugger.c $(CFLAGS)

display.o: display.c display.h
	$(CC) -c display.c $(CFLAGS) 

file_handler.o: file_handler.c file_handler.h
	$(CC) -c file_handler.c $(CFLAGS)

keyboard_state.o: keyboard_state.c keyboard_state.h
	$(CC) -c keyboard_state.c $(CFLAGS)

si_machine_io.o: si_machine_io.c si_machine_io.h
	$(CC) -c si_machine_io.c $(CFLAGS)

clean:
	rm -f main *.o