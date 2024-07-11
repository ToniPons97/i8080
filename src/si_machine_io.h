#ifndef SI_MACHINE_IO_H
#define SI_MACHINE_IO_H

#include "state8080.h"
#include "keyboard_state.h"

uint8_t si_machine_in(State8080* state, uint8_t port, KeyboardMap* keyboard_state);
void si_machine_out(State8080* state, uint8_t port);

#endif