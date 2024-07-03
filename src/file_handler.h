#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

void print_file(unsigned char* buffer, size_t size);
size_t get_file_size(FILE *file);
unsigned char* read_file(FILE* file, size_t buffer_size);
void read_file_at_offset(State8080* state, char* filename, uint32_t offset, size_t* size);
void load_space_invaders_rom(State8080* state, size_t* size);

#endif