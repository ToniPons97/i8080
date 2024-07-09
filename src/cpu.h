#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "state8080.h"
#include "io_interface.h"

void unimplemented_instruction(State8080* state);
bool emulate_i8080(State8080* state, IOInterface* io);
void call(State8080* state, uint16_t address);
void ret(State8080* state);
void push(State8080* state, uint8_t reg_hi, uint8_t reg_lo);
void pop(State8080* state, uint8_t* reg_hi, uint8_t* reg_lo);
int parity(uint8_t num);
void add_to_accumulator(State8080* state, uint8_t num);
void adc(State8080* state, uint8_t num);
void subtract_to_accumulator(State8080* state, uint8_t num);
void sbb(State8080* state, uint8_t num);
void ana(State8080* state, uint8_t num);
void xra(State8080* state, uint8_t num);
void ora(State8080* state, uint8_t num);
void cmp(State8080* state, uint8_t num);
void inr(State8080* state, uint8_t* reg);
void dcr(State8080* state, uint8_t* reg);
void dad(State8080* state, uint16_t num);

State8080* init_8080_state(void);

#endif