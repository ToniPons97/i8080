CC = gcc
CFLAGS = -Wall -lSDL2 -Iheaders
SRCDIR = ./src
HEADERS = ./headers
OBJECTS = main.o cpu.o debugger.o display.o file_handler.o keyboard_state.o si_machine_io.o

# Target to build the final executable
main: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

# Pattern rule for compiling .c files into .o files
%.o: $(SRCDIR)/%.c $(HEADERS)/%.h
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up build artifacts
clean:
	rm -f main *.o
