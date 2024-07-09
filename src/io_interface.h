#ifndef IO_INTERFACE_H
#define IO_INTERFACE_H

#include "state8080.h"

typedef struct IOInterface {
    uint8_t (*machine_in) (State8080* state, uint8_t port);
    void (*machine_out) (State8080* state, uint8_t port);
} IOInterface;

#endif