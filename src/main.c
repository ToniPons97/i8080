#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "i8080_disassembler.h"
#include "i8080_cpu.h"

void print_file(unsigned char* buffer, size_t size);
size_t get_file_size(FILE *file);
unsigned char* read_file(FILE* file, size_t buffer_size);
void read_file_at_offset(State8080* state, char* filename, uint32_t offset, size_t* size);
void print_cpu_status(State8080* state);
void set_raw_mode(struct termios *original);
void restore_mode(struct termios *original);
int handle_debugger_commands(State8080* state, char key);
void print_banner(void);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: ./%s space-invaders.bin\n", argv[0]);

        return 1;
    }

    struct termios original;
    char key;

    set_raw_mode(&original);

    FILE *file = NULL;
    size_t file_size = 0;

    print_banner();

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

    size_t size = 0;

    read_file_at_offset(state, "invaders.h", 0, &size);
    read_file_at_offset(state, "invaders.g", 0x800, &size);
    read_file_at_offset(state, "invaders.f", 0x1000, &size);
    read_file_at_offset(state, "invaders.e", 0x1800, &size);

    while(state->pc < size) {
        read(STDIN_FILENO, &key, 1);

        if (!handle_debugger_commands(state, key)) {
            disassemble8080Opcode(state->memory, state->pc);
            emulate_i8080(state);
        }
    }

    free(buffer);
    free(state);
    restore_mode(&original);

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

void read_file_at_offset(State8080* state, char* filename, uint32_t offset, size_t* size) {
    FILE* file = NULL;

    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file.");
        return;
    }

    size_t fsize = get_file_size(file);
    *size += fsize;
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, file);
	fclose(file);
}

void set_raw_mode(struct termios *original) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    *original = raw;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void restore_mode(struct termios *original) {
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}

int handle_debugger_commands(State8080* state, char key) {
    switch (key)
    {
    case 's':
        print_cpu_status(state);
        return 1;
    case 'n':
        printf("[NEXT] ");
        disassemble8080Opcode(state->memory, state->pc);
        return 1;
    case 'm':
        print_ram(state);
        return 1;
    case 'v':
        print_vram(state);
        return 1;
    case 'r':
        printf("Should rewind\n");
    default:
        return 0;
    }
}

void print_banner() {
    printf("==============================================================================\n");
    printf("=                                                                            =\n");
    printf("=                          Intel 8080 Emulator                               =\n");
    printf("=                       Disassembler and Debugger                            =\n");
    printf("=                                                                            =\n");
    printf("=                       Developed by Toni Pons                               =\n");
    printf("=                                                                            =\n");
    printf("==============================================================================\n\n");
}