#include "debugger.h"

void print_cpu_status(State8080* state) {
    printf("\n=========================== START OF CPU STATUS ===========================\n");
    printf("Registers:\n");
    printf("A:  0x%.2x    B:  0x%.2x    C:  0x%.2x\n", state->a, state->b, state->c);
    printf("D:  0x%.2x    E:  0x%.2x    H:  0x%.2x\n", state->d, state->e, state->h);
    printf("L:  0x%.2x    SP: 0x%.4x\n", state->l, state->sp);
    printf("\nFlags:\n");
    printf("Z:  0x%.2x    S:  0x%.2x    CY: 0x%.2x\n", state->cc.z, state->cc.s, state->cc.cy);
    printf("AC: 0x%.2x    P:  0x%.2x    PC: 0x%.4x\n", state->cc.ac, state->cc.p, state->pc);
    printf("\n=========================== END OF CPU STATUS ============================\n\n");
}

void print_ram(State8080* state) {
    printf("\n=========================== START OF RAM ===========================\n\n");
    printf("Address    Value\n");
    printf("-------    -----\n");

    int range = 10;
    int start = state->sp - range;
    int end = state->sp + range;

    for (int i = start; i <= end; i++) {
        if (i >= 0 && i < 0x10000) {
            if (i == state->sp)
                printf("*");
            else
                printf(" ");

            printf("0x%.4x    0x%.2x\n", i, state->memory[i]);
        } else {
            printf(" 0x%.4x    ----\n", i);
        }
    }

    printf("\n=========================== END OF RAM ============================\n\n");
}

void print_vram(State8080* state) {
    printf("\n=========================== START OF VRAM ===========================\n\n");
    printf("Address    Value\n");
    printf("-------    -----\n");

    for (int i = 0x2400; i < 0x3FFF; i++) {
        if (i >= 0 && i < 0x10000) {
            printf(" ");
            printf("0x%.4x    0x%.2x\n", i, state->memory[i]);
        } else {
            printf(" 0x%.4x    ----\n", i);
        }
    }

    printf("\n=========================== END OF VRAM ============================\n\n");
}

State8080 jump_to(State8080* state, uint16_t new_pc) {
    if (new_pc >= 0x10000) {  // Assuming maximum address space of 64KB
        printf("Invalid program counter\n");
        return *state;
    }

    State8080* new_state = init_8080_state();
    while (new_state->pc < new_pc) {
        emulate_i8080(new_state);
    }

    return *new_state;
}