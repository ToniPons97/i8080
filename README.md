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
gcc -c main.c -Wall
gcc -c i8080_disassembler.c -Wall
gcc -c i8080_cpu.c -Wall
gcc -c debugger.c -Wall
gcc -o main main.o i8080_disassembler.o i8080_cpu.o debugger.o -Wall
```

Using make:
```bash
make
```