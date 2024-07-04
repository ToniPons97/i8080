# Intel 8080 Emulator

This project aims to be a compact Intel 8080 emulator written in C. It includes a disassembler and a debugger for enhanced functionality and ease of use.

## Features

- **Emulator**: Accurately emulates the Intel 8080 CPU.
- **Disassembler**: Converts machine code back into assembly language.
- **Debugger**: Assists in testing and debugging code with various tools and features.

## Usage

To get started, compile the source code and run the emulator. The disassembler and debugger can be used to analyze and troubleshoot your programs effectively.

## Compilation

Use the following command to compile the emulator:

```bash
gcc -c main.c -Wall -lSDL2
gcc -c cpu.c -Wall -lSDL2
gcc -c debugger.c -Wall -lSDL2
gcc -c display.c -Wall -lSDL2
gcc -c file_handler.c -Wall -lSDL2
gcc -o main main.o cpu.o debugger.o display.o file_handler.o -Wall -lSDL2
```

Using make:
```bash
cd src
make
```