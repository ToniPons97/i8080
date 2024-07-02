#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "cpu.h"
#include "debugger.h"
#include "renderer.h"

void print_file(unsigned char* buffer, size_t size);
size_t get_file_size(FILE *file);
unsigned char* read_file(FILE* file, size_t buffer_size);
void read_file_at_offset(State8080* state, char* filename, uint32_t offset, size_t* size);
void print_cpu_status(State8080* state);
void set_raw_mode(struct termios *original);
void restore_mode(struct termios *original);
int handle_debugger_commands(State8080* state, char key);
void print_banner(void);
void load_space_invaders_rom(State8080* state, size_t* size);
uint16_t hex_to_decimal(char* hex_str);

size_t binary_buffer_size = 0;
int instruction_count = 1;
struct termios original;
SDL_Window* MAIN_WINDOW = NULL;
SDL_Renderer* MAIN_RENDERER = NULL;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: %s space-invaders.bin\n", argv[0]);

        return 1;
    }

    char key = '\0';

    set_raw_mode(&original);

    print_banner();

    State8080* state = init_8080_state();
    load_space_invaders_rom(state, &binary_buffer_size);

    initialize_sdl();
    MAIN_WINDOW = create_window();
    MAIN_RENDERER = create_renderer(MAIN_WINDOW);

    int counter = 0;
    while(state->pc < binary_buffer_size) {
        read(STDIN_FILENO, &key, 1);

        if (handle_debugger_commands(state, key)) {
            counter = instruction_count;
        } else {
            while (counter++ < instruction_count) {
                disassemble(state->memory, state->pc);
                emulate_i8080(state);
                render_screen(state->memory, MAIN_RENDERER);
            }
            
            counter = 0;
        }
    }

    printf("OUT OF LOOP\nProgram counter: 0x%.4x\n\n", state->pc);
    printf("IS STATE NULL? %p\n", state);

    sdl_cleanup(MAIN_WINDOW, MAIN_RENDERER);

    if (state == NULL) {
        free(state);
    }

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
    switch (key) {
        case 'q':
            sdl_cleanup(MAIN_WINDOW, MAIN_RENDERER);
            restore_mode(&original);
            exit(0);
        case 's':
            print_cpu_status(state);
            return 1;
        case 'n':
            printf("[NEXT] ");
            disassemble(state->memory, state->pc);
            return 1;
        case 'm':
            print_ram(state);
            return 1;
        case 'v':
            print_vram(state);
            return 1;
        case 'j':
            restore_mode(&original);
            char new_pc[8];
            uint16_t decimal_pc;

            printf("Enter new program counter: ");
            fgets(new_pc, 8, stdin);

            decimal_pc = hex_to_decimal(new_pc);

            printf("New pc: %s", new_pc);
            *state = jump_to(state, decimal_pc, load_space_invaders_rom, &binary_buffer_size);
            set_raw_mode(&original);
            return 1;
        case 'i':
            restore_mode(&original);
            printf("Enter number of instructions to execute: ");

            do {
                if (instruction_count < 1)
                    printf("Negative numbers aren't allowed: ");
                scanf("%d", &instruction_count);
            } while(instruction_count < 1);
            
            set_raw_mode(&original);
            return 1;
        default:
            return 0;
    }
}

void load_space_invaders_rom(State8080* state, size_t* size) {
    *size = 0;
    read_file_at_offset(state, "invaders.h", 0, size);
    read_file_at_offset(state, "invaders.g", 0x800, size);
    read_file_at_offset(state, "invaders.f", 0x1000, size);
    read_file_at_offset(state, "invaders.e", 0x1800, size);
    printf("\n");
}

uint16_t hex_to_decimal(char* hex_str) {
    unsigned long decimal = strtoul(hex_str, NULL, 16);
    return (uint16_t) decimal;
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