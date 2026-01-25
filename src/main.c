#define _GNU_SOURCE
#include "cpu.h"
#include "debugger.h"
#include "display.h"
#include "si_machine_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

void print_banner(void);
int handle_program_init(int argc, char** argv);
void debug_space_invaders(void);
void run_space_invaders(void);
void run_cpudiag(void);
static inline void detect_cpu_migration(int* last_cpu);
static void pin_cpu(int cpu);

struct termios original;
SDL_Window* MAIN_WINDOW = NULL;
SDL_Renderer* MAIN_RENDERER = NULL;
unsigned int instruction_count = 1;
size_t rom_buffer_size = 0;

int main(int argc, char** argv) {
    pin_cpu(19);
    print_banner();

    switch (handle_program_init(argc, argv)) {
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

void debug_space_invaders() 
{
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

        if (handle_debugger_commands(state, key, &instruction_count,
                                     &rom_buffer_size, &original, MAIN_WINDOW,
                                     MAIN_RENDERER)) {
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

#define SPACE_INVADERS_STACK_START 0x2400
#define TARGET_FPS 60

void run_space_invaders() 
{
    State8080* state = init_8080_state();
    load_space_invaders_rom(state, &rom_buffer_size);
    state->sp = SPACE_INVADERS_STACK_START;

    IOInterface io = {.machine_in = si_machine_in,
                      .machine_out = si_machine_out};

    KeyboardMap key_state;
    init_keyboard_state(&key_state);

    initialize_sdl();
    MAIN_WINDOW = create_window();
    MAIN_RENDERER = create_renderer(MAIN_WINDOW);

    bool quit = false;
    SDL_Event event;
    double T_STATES_PER_FRAME = 33333.33;
    bool int1_fired = false;
    bool vblank_occurred = false;
    int old_cpu_core = -1;
    uint32_t last_frame_ticks = SDL_GetTicks();

    while (!quit) {
        emulate_i8080(state, &io, &key_state);
        handle_sdl_events(&key_state, &event, &quit);

        // Handle interrupt: at most one per iteration. Check vblank first so we
        // never deliver mid-frame and vblank in the same iteration (which would
        // run the vblank handler before the mid-frame handler and break the game).
        if (state->interrupt_enable) {
            if (state->t_states >= T_STATES_PER_FRAME) {
                // Render the completed frame (including previous vblank handler's
                // updates) before delivering this vblank. This avoids showing a
                // frame where main code cleared VRAM but the vblank redraw hasn't run yet.
                render_screen(state->memory, MAIN_RENDERER);
                generate_interrupt(state, 2);
                state->t_states -= T_STATES_PER_FRAME;
                int1_fired = false;
                vblank_occurred = true;
                detect_cpu_migration(&old_cpu_core);
                keyboard_end_frame(&key_state);
            } else if (state->t_states > T_STATES_PER_FRAME / 2 && !int1_fired) {
                generate_interrupt(state, 1);
                int1_fired = true;
            }
        }

        if (vblank_occurred) {
            /* Throttle to ~60 FPS so game logic and timers run at correct speed. */
            uint32_t now = SDL_GetTicks();
            uint32_t frame_ms = 1000 / TARGET_FPS;
            int32_t delay = (int32_t)(last_frame_ticks + frame_ms) - (int32_t)now;
            if (delay > 0)
                SDL_Delay((uint32_t)delay);
            last_frame_ticks = SDL_GetTicks();
            vblank_occurred = false;
        }
        
    }

    if (state != NULL) {
        free(state->memory);
        free(state);
    }

    //sdl_cleanup(MAIN_WINDOW, MAIN_RENDERER);
}

void run_cpudiag(void) 
{
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
    state->memory[0xde02] = 0xAA;

    //(state->pc < buffer_size) &&
    while (!should_exit) {
        read(STDIN_FILENO, &key, 1);

        if (handle_debugger_commands(state, key, &instruction_count,
                                     &buffer_size, &original, MAIN_WINDOW,
                                     MAIN_RENDERER)) {
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

int handle_program_init(int argc, char** argv) 
{
    if (argc != 2 && argc != 3) {
        printf("[!] Binary not provided...\n");
        printf("Example usage: %s space-invaders.bin\n", argv[0]);
        return -1;
    }

    char invaders[] = "invaders";
    char debug_flag[] = "--debug";

    switch (argc) {
    case 2:
        if (strncmp(invaders, argv[1],
                    sizeof(invaders) / sizeof(invaders[0])) == 0) {
            printf("Running Space Invaders...\n");
            return 0;
        } else {
            printf("Should run some other file if it exists.\n");
            return -1;
        }

    case 3:
        if (strncmp(invaders, argv[1],
                    sizeof(invaders) / sizeof(invaders[0])) == 0 &&
            strncmp(debug_flag, argv[2],
                    sizeof(debug_flag) / sizeof(debug_flag[0])) == 0) {
            printf("Should run space invaders in debug mode.\n");
            return 1;
        } else if (strncmp(debug_flag, argv[2],
                           sizeof(debug_flag) / sizeof(debug_flag[0])) == 0) {
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

void print_banner(void) 
{
    printf("==================================================================="
           "===========\n");
    printf("=                                                                  "
           "          =\n");
    printf("=                          Intel 8080 Emulator                     "
           "          =\n");
    printf("=                       Disassembler and Debugger                  "
           "          =\n");
    printf("=                                                                  "
           "          =\n");
    printf("=                       Developed by Toni Pons                     "
           "          =\n");
    printf("=                                                                  "
           "          =\n");
    printf("==================================================================="
           "===========\n\n");
}

static inline void detect_cpu_migration(int* last_cpu) 
{
    int cpu_core = sched_getcpu();
    if (cpu_core != *last_cpu) {
        printf("Running on cpu core %d\n", cpu_core);
        *last_cpu = cpu_core;
    }
}

static void pin_cpu(int cpu)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    sched_setaffinity(0, sizeof(set), &set);
}
