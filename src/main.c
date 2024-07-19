#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "debugger.h"
#include "display.h"
#include "si_machine_io.h"
#include <sys/time.h>

void print_banner(void);
int handle_program_init(int argc, char** argv);
void debug_space_invaders(void);
void run_space_invaders(void);
void run_cpudiag(void);
long long get_current_time(void);

struct termios original;
SDL_Window* MAIN_WINDOW = NULL;
SDL_Renderer* MAIN_RENDERER = NULL;
unsigned int instruction_count = 1;
size_t rom_buffer_size = 0;

int main(int argc, char** argv) {
    print_banner();
    
    switch(handle_program_init(argc, argv)) {
        case 0:
            run_space_invaders();
            break;
        
        case 1:
            debug_space_invaders();
            break;

        default:
            printf("Running CPUDIAG\n\n");
            run_cpudiag();
            return 1;
    }

    return 0;
}

void debug_space_invaders() {
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

        if (handle_debugger_commands(state, key, &instruction_count, &rom_buffer_size, &original, MAIN_WINDOW, MAIN_RENDERER)) {
            counter = instruction_count;
        } else {
            while (counter++ < instruction_count) {
                disassemble(state->memory, state->pc);
                emulate_i8080(state, NULL, NULL);
                render_screen(state->memory, MAIN_RENDERER);
            }

            counter = 0;
        }
    }

    printf("OUT OF LOOP\nProgram counter: 0x%.4x\n\n", state->pc);
    printf("IS STATE NULL? %p\n", state);

    sdl_cleanup(MAIN_WINDOW, MAIN_RENDERER);

    if (state != NULL) {
        free(state->memory);
        free(state);
    }

    restore_mode(&original);
}

void run_space_invaders() {
    State8080* state = init_8080_state();
    load_space_invaders_rom(state, &rom_buffer_size);

    IOInterface io = {
        .machine_in = si_machine_in,
        .machine_out = si_machine_out
    };

    KeyboardMap key_state;
    init_keyboard_state(&key_state);

    initialize_sdl();
    MAIN_WINDOW = create_window();
    MAIN_RENDERER = create_renderer(MAIN_WINDOW);

    bool quit = false;
    SDL_Event event;
    long long last_interrupt_time = 0;
    long long next_interrupt_time = 0;
    int which_interrupt = 1;

    while (!quit) {
        handle_sdl_events(&key_state, &event, &quit);
        
        // Emulate CPU cycles until the next expected interrupt
        long long now = get_current_time();
        while (get_current_time() < next_interrupt_time) {
            emulate_i8080(state, &io, &key_state);
        }

        // Handle interrupt
        if (state->interrupt_enable) {
            if (which_interrupt == 1) {
                generate_interrupt(state, 1); // VBlank interrupt
                which_interrupt = 2;
            } else {
                generate_interrupt(state, 2); // Mid-frame interrupt
                which_interrupt = 1;
            }
        }

        // Update timing for next interrupt
        last_interrupt_time = now;
        next_interrupt_time = last_interrupt_time + 8000; // 8ms for 2 interrupts in 16ms frame

        // Render the screen at VBlank
        if (which_interrupt == 1) {
            render_screen(state->memory, MAIN_RENDERER);
        }
    }

    if (state != NULL) {
        free(state->memory);
        free(state);
    }

    sdl_cleanup(MAIN_WINDOW, MAIN_RENDERER);
}



void run_cpudiag(void) {
    set_raw_mode(&original);

    State8080* state = init_8080_state();

    bool should_exit = false;
    size_t buffer_size = 0;
    char key = '\0';
    int counter = 0;

    read_file_at_offset(state, "./cpudiag", 0x100, &buffer_size);
    printf("Buffer size: %ld\n", buffer_size);
    
    state->pc = 0x100;
    state->memory[368] = 0x7; 

    while ((state->pc < buffer_size) && !should_exit) {
        read(STDIN_FILENO, &key, 1);

        if (handle_debugger_commands(state, key, &instruction_count, &buffer_size, &original, MAIN_WINDOW, MAIN_RENDERER)) {
            counter = instruction_count;
        } else {
            while (counter++ < instruction_count) {
                disassemble(state->memory, state->pc);
                should_exit = emulate_i8080(state, NULL, NULL);
            }

            counter = 0;
        }
    }

    if (state != NULL) {
        free(state->memory);
        free(state);
    }

    restore_mode(&original);
}

int handle_program_init(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: %s space-invaders.bin\n", argv[0]);
        return -1;
    }
    
    char invaders[] = "invaders";
    char debug_flag[] = "--debug";

    switch (argc) {
        case 2:
            if (strncmp(invaders, argv[1], sizeof(invaders) / sizeof(invaders[0])) == 0) {
                printf("Running Space Invaders...\n");
                return 0;
            } else {
                printf("Should run some other file if it exists.\n");
                return -1;
            }
        
        case 3:
            if (strncmp(invaders, argv[1], sizeof(invaders) / sizeof(invaders[0])) == 0 && 
                strncmp(debug_flag, argv[2], sizeof(debug_flag) / sizeof(debug_flag[0])) == 0) {
                    printf("Should run space invaders in debug mode.\n");
                    return 1;
            } else if (strncmp(debug_flag, argv[2], sizeof(debug_flag) / sizeof(debug_flag[0])) == 0) {
                printf("Should run some other file in debug mode if it exists.\n");
                return -1;
            } else {
                printf("Should tell you to fuck off.\n");
                return -1;
            }

        default:
            return -1;
    }
}

long long get_current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long) tv.tv_sec * 1000000LL) + (tv.tv_usec);
}

void print_banner(void) {
    printf("==============================================================================\n");
    printf("=                                                                            =\n");
    printf("=                          Intel 8080 Emulator                               =\n");
    printf("=                       Disassembler and Debugger                            =\n");
    printf("=                                                                            =\n");
    printf("=                       Developed by Toni Pons                               =\n");
    printf("=                                                                            =\n");
    printf("==============================================================================\n\n");
}