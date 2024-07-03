#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "debugger.h"
#include "display.h"

void print_banner(void);

struct termios original;
SDL_Window* MAIN_WINDOW = NULL;
SDL_Renderer* MAIN_RENDERER = NULL;
unsigned int instruction_count = 1;
size_t rom_buffer_size = 0;


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: %s space-invaders.bin\n", argv[0]);
        return 1;
    }

    print_banner();

    char key = '\0';
    set_raw_mode(&original);

    State8080* state = init_8080_state();
    load_space_invaders_rom(state, &rom_buffer_size);

    initialize_sdl();
    MAIN_WINDOW = create_window();
    MAIN_RENDERER = create_renderer(MAIN_WINDOW);

    bool quit = false;
    SDL_Event event;

    int counter = 0;
    while (!quit && state->pc < rom_buffer_size) {
        handle_quit_event(&event, &quit);

        read(STDIN_FILENO, &key, 1);

        if (handle_debugger_commands(state, key, instruction_count, &rom_buffer_size, &original, MAIN_WINDOW, MAIN_RENDERER)) {
            counter = instruction_count;
        } else {
            while (counter++ < instruction_count) {
                disassemble(state->memory, state->pc);
                emulate_i8080(state);
                render_screen(state->memory, MAIN_RENDERER);
                //SDL_Delay(1000 / 60);
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