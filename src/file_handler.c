#include "file_handler.h"

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

void load_space_invaders_rom(State8080* state, size_t* size) {
    *size = 0;
    read_file_at_offset(state, "invaders.h", 0, size);
    read_file_at_offset(state, "invaders.g", 0x800, size);
    read_file_at_offset(state, "invaders.f", 0x1000, size);
    read_file_at_offset(state, "invaders.e", 0x1800, size);
    printf("\n");
}