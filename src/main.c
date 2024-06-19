#include <stdio.h>
#include <stdlib.h>
#include "i8080_disassembler.h"
#include "i8080_cpu.h"

void print_file(unsigned char* buffer, size_t size);
size_t get_file_size(FILE *file);
unsigned char* read_file(FILE* file, size_t buffer_size);
void read_file_at_offset(State8080* state, char* filename, uint32_t offset);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: ./%s space-invaders.bin\n", argv[0]);

        return 1;
    }

    FILE *file = NULL;
    size_t file_size = 0;

    printf("Dissasembling %s\n", argv[1]);

    file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Unable to read file.");
        return 1;
    }

    file_size = get_file_size(file);
    if (file_size == -1) {
        perror("Error reading buffer size. Exiting program.");
        return 1;
    }

    unsigned char* buffer = read_file(file, file_size);

    State8080* state = init_8080_state();

    read_file_at_offset(state, "invaders.h", 0);
    read_file_at_offset(state, "invaders.g", 0x800);
    read_file_at_offset(state, "invaders.f", 0x1000);
    read_file_at_offset(state, "invaders.e", 0x1800);

    int pc = 0;
    char stepper = '\0';
    while(state->pc < file_size) {
        scanf("%c", &stepper);

        if (stepper != 's') {
            pc += disassemble8080Opcode(state->memory, state->pc); 
            emulate_i8080(state);
        } else {
            print_cpu_status(state);
        }
    }

    free(buffer);
    free(state);

    return 0;
}

void print_file(unsigned char* buffer, size_t size) {
    for(int i = 0; i < size; i++)
        printf("%.2x ", buffer[i]);
}

size_t get_file_size(FILE* file) {
    size_t size;

    if (file == NULL) {
        perror("File pointer is null.");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    return size;
}

unsigned char* read_file(FILE* file, size_t buffer_size) {
    unsigned char* buffer = NULL;
    buffer = (unsigned char*) calloc(buffer_size, sizeof(unsigned char));
    if (buffer == NULL) {
        perror("could not allocate memory dynamically.");
        return NULL;
    }

    fread(buffer, buffer_size, 1, file);
    fclose(file);

    return buffer;
}

void read_file_at_offset(State8080* state, char* filename, uint32_t offset) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

void print_cpu_status(State8080* state) {
    printf("\nA: 0x%.2x\nB: 0x%.2x\nC: 0x%.2x\nD: 0x%.2x\nE: 0x%.2x\nH: 0x%.2x\nL: 0x%.2x\nSP: 0x%.4x\n", 
        state->a, state->b, state->c, state->d, state->e, state->h, state->l, state->sp);
    
    printf("\nZ: 0x%.2x\nS: 0x%.2x\nCY: 0x%.2x\nAC: 0x%.2x\nP: 0x%.2x\n\n", 
        state->cc.z, state->cc.s, state->cc.cy, state->cc.ac, state->cc.p);
}