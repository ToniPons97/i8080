#ifndef IO_INTERFACE_H
#define IO_INTERFACE_H

#include "state8080.h"
#include "keyboard_state.h"

typedef struct IOInterface {
    uint8_t (*machine_in) (State8080* state, uint8_t port, KeyboardMap* keyboard_state);
    void (*machine_out) (State8080* state, uint8_t port);
} IOInterface;

#endif