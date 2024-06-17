#include <stdint.h>

#ifndef I8080_CPU
#define I8080_CPU

typedef struct ConditionCodes {    
    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    uint8_t    ac:1;    
    uint8_t    pad:3;    
} ConditionCodes;

typedef struct State8080 {    
    uint8_t    a;    
    uint8_t    b;    
    uint8_t    c;    
    uint8_t    d;    
    uint8_t    e;    
    uint8_t    h;    
    uint8_t    l;    
    uint16_t    sp;    
    uint16_t    pc;    
    uint8_t     *memory;    

    struct ConditionCodes  cc;    
    uint8_t     int_enable;    
} State8080;

void unimplemented_instruction(State8080* state);
void emulate_i8080_op(State8080* state);
void call(State8080* state, uint16_t address);
void ret(State8080* state);
void push(State8080* state, uint8_t reg_hi, uint8_t reg_lo);
void pop(State8080* state, uint8_t* reg_hi, uint8_t* reg_lo);
int parity(uint8_t num);
void add_to_accumulator(State8080* state, uint16_t num);
void adc(State8080* state, uint16_t num);

#endif