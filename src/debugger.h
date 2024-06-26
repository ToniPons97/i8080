#include "i8080_cpu.h"
#include <stdio.h>


void print_cpu_status(State8080* state);
void print_ram(State8080* state);
void print_vram(State8080* state);
State8080 jump_to(State8080* state, uint16_t new_pc, void (*load_code)(State8080*, size_t*), size_t* size);