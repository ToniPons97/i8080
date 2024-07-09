#ifndef STATE_8080_H
#define STATE_8080_H

#include <stdint.h>

typedef struct ConditionCodes {    
    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    uint8_t    ac:1;    
    uint8_t    pad:3;    
} ConditionCodes;

typedef struct ShiftRegisters {
    uint8_t shift0;
    uint8_t shift1;
    uint8_t shift_offset;
} ShiftRegisters;

typedef struct ExternalDevices {
    ShiftRegisters shift_registers;
} ExternalDevices;

typedef struct State8080 {    
    uint8_t    a;    
    uint8_t    b;    
    uint8_t    c;    
    uint8_t    d;    
    uint8_t    e;    
    uint8_t    h;    
    uint8_t    l;    
    uint16_t   sp;    
    uint16_t   pc;    
    uint8_t*   memory;

    struct ConditionCodes  cc;    
    uint8_t     int_enable;


    // Space Invaders Dedicated Shift Hardware
    ExternalDevices external_devices;
} State8080;

#endif