#include <stdio.h>
#include <stdlib.h>
#include "i8080_disassembler.h"

void print_file(unsigned char* buffer, size_t size);
size_t get_file_size(FILE *file);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: ./%s pong.bin\n", argv[0]);

        return 1;
    }

    unsigned char* buffer = NULL;
    FILE *file = NULL;
    size_t file_buff_size = 0;

    printf("Dissasembling %s\n", argv[1]);

    file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Unable to read file.");
        return 1;
    }

    file_buff_size = get_file_size(file);
    if (file_buff_size == -1) {
        perror("Error getting buffer size. Exiting program.");
        return 1;
    }

    buffer = (unsigned char*) calloc(file_buff_size, sizeof(unsigned char));
    if (buffer == NULL) {
        perror("could not allocate memory dynamically.");
        return 1;
    }

    fread(buffer, file_buff_size, 1, file);

    for (int i = 0; i < file_buff_size; i++) {
        disassemble8080Opcode(buffer, i);
    }

    free(buffer);
    fclose(file);

    return 0;
}

void print_file(unsigned char* buffer, size_t size) {
    for(int i = 0; i < size; i++)
        printf("%.2x ", buffer[i]);
}

size_t get_file_size(FILE *file) {
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