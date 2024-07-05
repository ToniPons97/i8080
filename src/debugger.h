#ifndef DEBUGGER
#define DEBUGGER

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "cpu.h"
#include "file_handler.h"
#include "display.h"

int disassemble(unsigned char *buffer, int pc);
void print_cpu_status(State8080* state);
void print_ram(State8080* state);
void print_vram(State8080* state);
uint16_t hex_to_decimal(char* hex_str);
void set_raw_mode(struct termios *original);
void restore_mode(struct termios *original);
bool handle_debugger_commands(State8080* state, char key, unsigned int* instruction_count, size_t* buffer_size, struct termios* terminal, SDL_Window* window, SDL_Renderer* renderer);
State8080 jump_to(State8080* state, uint16_t new_pc, void (*load_code)(State8080*, size_t*), size_t* size);

#endif