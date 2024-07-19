#include "cpu.h"

// https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf
// https://deramp.com/downloads/intel/8080%20Data%20Sheet.pdf

void unimplemented_instruction(State8080* state) {    
    state->pc -= 1;

    printf ("Error: Unimplemented instruction\n");    
    exit(1);    
}

uint16_t last_sp = 0;

bool emulate_i8080(State8080* state, IOInterface* io, KeyboardMap* keyboard_state) {    
    unsigned char *opcode = &state->memory[state->pc];
    /*
    if ((state->sp != 0) && (state->sp < 0x2300)) {
        printf("Stack getting dangerously low 0x%.4x\n", state->sp);    
    } 
    if ( abs(last_sp - state->sp) > 2) {
        printf("\nStack Squash?\n\nPC: 0x%.4x\nLast SP: 0x%.4x\nCurrent SP: 0x%.4x\n\n", state->pc, last_sp, state->sp);    
        last_sp = state->sp;
    }
    */


    if (state->pc == 0x8) {
        printf("RST 1\n");
    } 
    
    if (state->pc == 0x10) {
        printf("RST 2\n");    
    }
    
    if (state->pc == 0x87) {
        printf("Leaving RST\n");
    }

    switch(*opcode) {    
        case 0x00: state->t_states += 4; break;           // NOP    
        case 0x01: {                  // LXI B,D16
            state->c = opcode[1];
            state->b = opcode[2];
            state->pc += 2;
            state->t_states += 10;
            break;
        }  
        case 0x02: {                  // STAX B
            uint16_t address = (state->b << 8) | state->c;
            state->memory[address] = state->a;
            state->t_states += 7;
            break;
        }   
        case 0x03: {                  // INX B
            uint16_t bc = (state->b << 8) | state->c;
            bc += 1;
            state->b = (bc >> 8) & 0xff;
            state->c = bc & 0xff;
            state->t_states += 5;
            break; 
        }
        case 0x04: inr(state, &state->b); state->t_states += 5; break;            // INR B  
        case 0x05: dcr(state, &state->b); state->t_states += 5; break;            // DCR B
        case 0x06: {                  // MVI B, D8
            state->b = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }   
        case 0x07: {                  // RLC
            state->cc.cy = (state->a & 0x80) != 0;
            state->a = (state->a << 1) | (state->a >> 7);
            state->t_states += 4;
            break;
        }
        case 0x08:  break;    
        case 0x09: {                  // DAD B
            uint16_t bc = (state->b << 8) | state->c;
            dad(state, bc);
            state->t_states += 10;
            break;
        }  
        case 0x0a: {                  // LDAX B 
            uint16_t address = (state->b << 8) | state->c;
            state->a = state->memory[address];
            state->t_states += 7;
            break;
        }   
        case 0x0b: {                  // DCX B
            uint16_t bc = (state->b << 8) | state->c;
            bc -= 1;
            state->b = (bc >> 8) & 0xff;
            state->c = bc & 0xff;
            state->t_states += 5;
            break;    
        }
        case 0x0c: inr(state, &state->c); state->t_states += 5; break;            // INR C
        case 0x0d: dcr(state, &state->c); state->t_states += 5; break;            // DCR C  
        case 0x0e: {                  // MVI C,D8 
            state->c = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }   
        case 0x0f: {                  // RRC
            uint8_t lsb = state->a & 0x01;
            state->cc.cy = lsb;
            state->a = (state->a >> 1) | (lsb << 7);
            state->t_states += 4;
            break;
        }
        case 0x10: break;    
        case 0x11: {                  // LXI D,D16
            state->d = opcode[2];
            state->e = opcode[1];
            state->pc += 2;
            state->t_states += 10;
            break; 
        }   
        case 0x12: {                  // STAX D
            uint16_t address = (state->d << 8) | state->e;
            state->memory[address] = state->a;
            state->t_states += 7;
            break;
        }
        case 0x13: {                  // INX D
            uint16_t de = (state->d << 8) | state->e;
            de += 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            state->t_states += 5;
            break;
        }    
        case 0x14: inr(state, &state->d); state->t_states += 5; break;            // INR D   
        case 0x15: dcr(state, &state->d); state->t_states += 5; break;            // DCR D    
        case 0x16: {                  // MVI D, D8
            state->d = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }    
        case 0x17: {                  // RAL
            uint8_t msb = (state->a & 0x80) != 0;
            state->a = (state->a << 1) | (state->cc.cy << 7);
            state->cc.cy = msb;
            state->t_states += 4;
            break;
        }   
        case 0x18: break;    
        case 0x19: {                  // DAD D
            uint16_t de = (state->d << 8) | state->e;
            dad(state, de);
            state->t_states += 10;
            break;
        }   
        case 0x1a: {                  // LDAX D 
            uint16_t address = (state->d << 8) | state->e;
            state->a = state->memory[address];
            state->t_states += 7;
            break;
        }
        case 0x1b: {                  // DCX D
            uint16_t de = (state->d << 8) | state->e;
            de -= 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            state->t_states += 5;
            break;
        }    
        case 0x1c: inr(state, &state->e); state->t_states += 5; break;            // INR E
        case 0x1d: dcr(state, &state->e); state->t_states += 5; break;            // DCR E
        case 0x1e: {                  // MVI E,D8 
            state->e = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }    
        case 0x1f: {                  // RAR
            uint8_t lsb = state->a & 0x01;
            state->a = (state->a >> 1) | (state->cc.cy << 7);
            state->cc.cy = lsb;
            state->t_states += 4;
            break;
        }
        case 0x20: break;    
        case 0x21: {                  // LXI H,D16
            state->h = opcode[2];
            state->l = opcode[1];
            state->pc += 2;
            state->t_states += 10;
            break;
        }    
        case 0x22: {                  // SHLD addr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->memory[address] = state->l;
            state->memory[address + 1] = state->h;
            state->pc += 2;
            state->t_states += 16;
            break;
        }   
        case 0x23: {                  // INX H
            uint16_t hl = (state->h << 8) | state->l;
            hl += 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            state->t_states += 5;
            break;
        }    
        case 0x24: inr(state, &state->h); state->t_states += 5; break;            // INR H   
        case 0x25: dcr(state, &state->h); state->t_states += 5; break;            // DCR H    
        case 0x26: {                  // MVI H,D8 
            state->h = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }

        case 0x27: {                          //DAA    
            if ((state->a &0xf) > 9) {
                state->a += 6;
            }

            if ((state->a&0xf0) > 0x90) {    
                uint16_t res = (uint16_t) state->a + 0x60;    
                state->a = res & 0xff;

                state->cc.cy = res > 0xff;
                state->cc.p = parity(state->a);
                state->cc.z = (state->a & 0xff) == 0;
                state->cc.s = (state->a & 0x80) != 0;
            }

            state->t_states += 4;
            break;
        }
 
        case 0x28: break;    
        case 0x29: {                  // DAD H
            uint16_t hl = (state->h << 8) | state->l;
            dad(state, hl);
            state->t_states += 10;
            break;
        }    
        case 0x2a: {                  // LHLD addr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->l = state->memory[address];
            state->h = state->memory[address + 1];
            state->pc += 2;
            state->t_states += 16;
            break;
        }    
        case 0x2b: {                  // DCX H
            uint16_t hl = (state->h << 8) | state->l;
            hl -= 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            state->t_states += 5;
            break;
        }   
        case 0x2c: inr(state, &state->l); state->t_states += 5; break;            // INR L  
        case 0x2d: dcr(state, &state->l); state->t_states += 5; break;            // DCR L    
        case 0x2e: {                  // MVI L, D8 
            state->l = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }    
        case 0x2f: state->a = ~state->a; state->t_states += 4; break;              // CMA 
        case 0x30: break;    
        case 0x31: {                  // LXI SP, D16
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->sp = address;
            state->pc += 2;
            state->t_states += 10;
            break;
        }  
        case 0x32: {                  // STA addr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->memory[address] = state->a;
            state->pc += 2;
            state->t_states += 13;
            break;
        }    
        case 0x33: state->sp += 1; state->t_states += 5; break;                   // INX SP    
        case 0x34: {                  // INR M
            uint16_t address = (state->h << 8) | state->l;
            inr(state, &state->memory[address]);
            state->t_states += 10;
            break;
        }    
        case 0x35: {                  // DCR M
            uint16_t address = (state->h << 8) | state->l;
            dcr(state, &state->memory[address]);
            state->t_states += 10;
            break;
        }    
        case 0x36: {                  // MVI M,D8
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = opcode[1];
            state->pc += 1;
            state->t_states += 10;
            break;
        }
        case 0x37: state->cc.cy = 1; state->t_states += 4; break;                    // STC    
        case 0x38: break;    
        case 0x39: dad(state, state->sp); state->t_states += 10; break;            // DAD SP 
        case 0x3a: {                  // LDA addr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->a = state->memory[address];
            state->pc += 2;
            state->t_states += 13;
            break;
        }    
        case 0x3b: {                  // DCX SP
            state->sp -= 1;
            state->t_states += 5;
            break;
        }    
        case 0x3c: inr(state, &state->a); state->t_states += 5; break;               // INR A
        case 0x3d: dcr(state, &state->a); state->t_states += 5; break;               // DCR A
        case 0x3e: {                  // MVI A,D8
            state->a = opcode[1];
            state->pc += 1;
            state->t_states += 7;
            break;
        }    
        case 0x3f: state->cc.cy = ~state->cc.cy; state->t_states += 4; break;     // CMC   
        case 0x40: state->b = state->b; state->t_states += 5; break;              // MOV B,B  
        case 0x41: state->b = state->c; state->t_states += 5; break;              // MOV B,C    
        case 0x42: state->b = state->d; state->t_states += 5; break;              // MOV B,D   
        case 0x43: state->b = state->e; state->t_states += 5; break;              // MOV B,E 
        case 0x44: state->b = state->h; state->t_states += 5; break;              // MOV B,H   
        case 0x45: state->b = state->l; state->t_states += 5; break;              // MOV B,L    
        case 0x46: {                  // MOV B,M
            uint16_t address = (state->h << 8) | state->l;
            state->b = state->memory[address];
            state->t_states += 7;
            break;
        }    
        case 0x47: state->b = state->a; state->t_states += 5; break;              // MOV B,A   
        case 0x48: state->c = state->b; state->t_states += 5; break;              // MOV C,B   
        case 0x49: state->c = state->c; state->t_states += 5; break;              // MOV C,C  
        case 0x4a: state->c = state->d; state->t_states += 5; break;              // MOV C,D
        case 0x4b: state->c = state->e; state->t_states += 5; break;              // MOV C,E   
        case 0x4c: state->c = state->h; state->t_states += 5; break;              // MOV C,H  
        case 0x4d: state->c = state->l; state->t_states += 5; break;              // MOV C,L    
        case 0x4e: {                  // MOV C,M
            uint16_t address = (state->h << 8) | state->l;
            state->c = state->memory[address];
            state->t_states += 7;
            break; 
        }   
        case 0x4f: state->c = state->a; state->t_states += 5; break;              // MOV C,A  
        case 0x50: state->d = state->b; state->t_states += 5; break;              // MOV D,B   
        case 0x51: state->d = state->c; state->t_states += 5; break;              // MOV D,C    
        case 0x52: state->d = state->d; state->t_states += 5; break;              // MOV D,D  
        case 0x53: state->d = state->e; state->t_states += 5; break;              // MOV D,E  
        case 0x54: state->d = state->h; state->t_states += 5; break;              // MOV D,H  
        case 0x55: state->d = state->l; state->t_states += 5; break;              // MOV D,L   
        case 0x56: {                  // MOV D,M
            uint16_t address = (state->h << 8) | state->l;
            state->d = state->memory[address];
            state->t_states += 7;
            break;
        }    
        case 0x57: state->d = state->a; state->t_states += 5; break;              // MOV D,A
        case 0x58: state->e = state->b; state->t_states += 5; break;              // MOV E,B
        case 0x59: state->e = state->c; state->t_states += 5; break;              // MOV E,C
        case 0x5a: state->e = state->d; state->t_states += 5; break;              // MOV E,D  
        case 0x5b: state->e = state->e; state->t_states += 5; break;              // MOV E,E 
        case 0x5c: state->e = state->h; state->t_states += 5; break;              // MOV E,H
        case 0x5d: state->e = state->l; state->t_states += 5; break;              // MOV E,L
        case 0x5e: {                  // MOV E,M
            uint16_t address = (state->h << 8) | state->l;
            state->e = state->memory[address];
            state->t_states += 7;
            break;
        }    
        case 0x5f: state->e = state->a; state->t_states += 5; break;              // MOV E,A
        case 0x60: state->h = state->b; state->t_states += 5; break;              // MOV H,B
        case 0x61: state->h = state->c; state->t_states += 5; break;              // MOV H,C
        case 0x62: state->h = state->b; state->t_states += 5; break;              // MOV H,D
        case 0x63: state->h = state->e; state->t_states += 5; break;              // MOV H,E
        case 0x64: state->h = state->h; state->t_states += 5; break;              // MOV H,H
        case 0x65: state->h = state->l; state->t_states += 5; break;              // MOV H,L
        case 0x66: {                 // MOV H,M
            uint16_t address = (state->h << 8) | state->l;
            state->h = state->memory[address];
            state->t_states += 7;
            break;  
        }  
        case 0x67: state->h = state->a; state->t_states += 5; break;              // MOV H,A
        case 0x68: state->l = state->b; state->t_states += 5; break;              // MOV L,B
        case 0x69: state->l = state->c; state->t_states += 5; break;              // MOV L,C
        case 0x6a: state->l = state->d; state->t_states += 5; break;              // MOV L,D
        case 0x6b: state->l = state->e; state->t_states += 5; break;              // MOV L,E
        case 0x6c: state->l = state->h; state->t_states += 5; break;              // MOV L,H
        case 0x6d: state->l = state->l; state->t_states += 5; break;              // MOV L,L
        case 0x6e: {                 // MOV L,M
            uint16_t address = (state->h << 8) | state->l;
            state->l = state->memory[address];
            state->t_states += 7;
            break;
        }    
        case 0x6f: state->l = state->a; state->t_states += 5; break;              // MOV L,A
        case 0x70: {                  // MOV M,B
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->b;
            state->t_states += 7;
            break;
        }    
        case 0x71: {                 // MOV M,C
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->c;
            state->t_states += 7;
            break;
        }    
        case 0x72: {                 // MOV M,D
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->d;
            state->t_states += 7;
            break;
        }    
        case 0x73: {                 // MOV M,E
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->e;
            state->t_states += 7;
            break;
        }    
        case 0x74: {                 // MOV M,H
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->h;
            state->t_states += 7;
            break;
        }    
        case 0x75: {                 // MOV M,L
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->l;
            state->t_states += 7;
            break;
        }    
        case 0x76: printf("HTL\n"); state->t_states += 7; return true;   // HLT
        case 0x77: {                 // MOV M,A
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->a;
            state->t_states += 7;
            break;
        }    
        case 0x78: state->a = state->b; state->t_states += 5; break;    // MOV A,B
        case 0x79: state->a = state->c; state->t_states += 5; break;    // MOV A,C
        case 0x7a: state->a = state->d; state->t_states += 5; break;    // MOV A,D
        case 0x7b: state->a = state->e; state->t_states += 5; break;    // MOV A,E
        case 0x7c: state->a = state->h; state->t_states += 5; break;    // MOV A,H
        case 0x7d: state->a = state->l; state->t_states += 5; break;    // MOV A,L
        case 0x7e: {                 // MOV A,M
            uint16_t address = (state->h << 8) | state->l;
            state->a = state->memory[address];
            state->t_states += 7;
            break;
        }    
        case 0x7f: state->a = state->a; state->t_states += 5; break;    // MOV A,A  
        case 0x80: add_to_accumulator(state, state->b); state->t_states += 4; break;    // ADD B 
        case 0x81: add_to_accumulator(state, state->c); state->t_states += 4; break;    // ADD C
        case 0x82: add_to_accumulator(state, state->d); state->t_states += 4; break;    // ADD D
        case 0x83: add_to_accumulator(state, state->e); state->t_states += 4; break;    // ADD E
        case 0x84: add_to_accumulator(state, state->h); state->t_states += 4; break;    // ADD H
        case 0x85: add_to_accumulator(state, state->l); state->t_states += 4; break;    // ADD L
        case 0x86: {                 // 	ADD M
            uint16_t address = (state->h << 8) | state->l;
            add_to_accumulator(state, state->memory[address]);
            state->t_states += 7;
            break; 
        }   
        case 0x87: add_to_accumulator(state, state->a); break;    // ADD A
        case 0x88: adc(state, state->b); state->t_states += 4; break;    // ADC B
        case 0x89: adc(state, state->c); state->t_states += 4; break;    // ADC C
        case 0x8a: adc(state, state->d); state->t_states += 4; break;    // ADC D
        case 0x8b: adc(state, state->e); state->t_states += 4; break;    // ADC E
        case 0x8c: adc(state, state->h); state->t_states += 4; break;    // ADC H
        case 0x8d: adc(state, state->l); state->t_states += 4; break;    // ADC L
        case 0x8e: {                 // ADC M
            uint16_t address = (state->h << 8) | state->l;
            adc(state, state->memory[address]);
            state->t_states += 7;
            break;
        }    
        case 0x8f: adc(state, state->a); state->t_states += 4; break;    // ADC A  
        case 0x90: subtract_to_accumulator(state, state->b); state->t_states += 4; break;    // SUB B
        case 0x91: subtract_to_accumulator(state, state->c); state->t_states += 4; break;    // SUB C
        case 0x92: subtract_to_accumulator(state, state->d); state->t_states += 4; break;    // SUB D
        case 0x93: subtract_to_accumulator(state, state->e); state->t_states += 4; break;    // SUB E
        case 0x94: subtract_to_accumulator(state, state->h); state->t_states += 4; break;    // SUB H
        case 0x95: subtract_to_accumulator(state, state->l); state->t_states += 4; break;    // SUB L
        case 0x96: {                 // SUB M
            uint16_t address = (state->h << 8) | state->l;
            subtract_to_accumulator(state, state->memory[address]);
            state->t_states += 7;
            break; 
        }   
        case 0x97: subtract_to_accumulator(state, state->a); state->t_states += 4; break;    // SUB A
        case 0x98: sbb(state, state->b); state->t_states += 4; break;    // SBB B
        case 0x99: sbb(state, state->c); state->t_states += 4; break;    // SBB C
        case 0x9a: sbb(state, state->d); state->t_states += 4; break;    // SBB D
        case 0x9b: sbb(state, state->e); state->t_states += 4; break;    // SBB E
        case 0x9c: sbb(state, state->h); state->t_states += 4; break;    // SBB H
        case 0x9d: sbb(state, state->l); state->t_states += 4; break;    // SBB L
        case 0x9e: {                 // SBB M
            uint16_t address = (state->h << 8) | state->l;
            sbb(state, state->memory[address]);
            state->t_states += 7;
            break;   
        } 
        case 0x9f: sbb(state, state->a); state->t_states += 4; break;    // SBB A
        case 0xa0: ana(state, state->b); state->t_states += 4; break;    // ANA B
        case 0xa1: ana(state, state->c); state->t_states += 4; break;    // ANA C
        case 0xa2: ana(state, state->d); state->t_states += 4; break;    // ANA D
        case 0xa3: ana(state, state->e); state->t_states += 4; break;    // ANA E
        case 0xa4: ana(state, state->h); state->t_states += 4; break;    // ANA H
        case 0xa5: ana(state, state->l); state->t_states += 4; break;    // ANA L
        case 0xa6: {                 // ANA M
            uint16_t address = (state->h << 8) | state->l;
            ana(state, state->memory[address]);
            state->t_states += 7;
            break;  
        }  
        case 0xa7: ana(state, state->a); state->t_states += 4; break;    // ANA A
        case 0xa8: xra(state, state->b); state->t_states += 4; break;    // XRA B
        case 0xa9: xra(state, state->c); state->t_states += 4; break;    // XRA C
        case 0xaa: xra(state, state->d); state->t_states += 4; break;    // XRA D
        case 0xab: xra(state, state->e); state->t_states += 4; break;    // XRA E
        case 0xac: xra(state, state->h); state->t_states += 4; break;    // XRA H
        case 0xad: xra(state, state->l); state->t_states += 4; break;    // XRA L
        case 0xae: {                  // XRA M
            uint16_t address = (state->h << 8) | state->l;
            xra(state, state->memory[address]);
            state->t_states += 7;
            break;
        }    
        case 0xaf: xra(state, state->a); state->t_states += 4; break;    // XRA A
        case 0xb0: ora(state, state->b); state->t_states += 4; break;    // ORA B
        case 0xb1: ora(state, state->c); state->t_states += 4; break;    // ORA C
        case 0xb2: ora(state, state->d); state->t_states += 4; break;    // ORA D
        case 0xb3: ora(state, state->e); state->t_states += 4; break;    // ORA E
        case 0xb4: ora(state, state->h); state->t_states += 4; break;    // ORA H
        case 0xb5: ora(state, state->l); state->t_states += 4; break;    // ORA L
        case 0xb6: {                 // ORA M
            uint16_t address = (state->h << 8) | state->l;
            ora(state, state->memory[address]);
            state->t_states += 7;
            break;  
        }  
        case 0xb7: ora(state, state->a); state->t_states += 4; break;    // ORA A
        case 0xb8: cmp(state, state->b); state->t_states += 4; break;    // CMP B
        case 0xb9: cmp(state, state->c); state->t_states += 4; break;    // CMP C
        case 0xba: cmp(state, state->d); state->t_states += 4; break;    // CMP D
        case 0xbb: cmp(state, state->e); state->t_states += 4; break;    // CMP E
        case 0xbc: cmp(state, state->h); state->t_states += 4; break;    // CMP H
        case 0xbd: cmp(state, state->l); state->t_states += 4; break;    // CMP L
        case 0xbe: {                 // CMP M
            uint16_t address = (state->h << 8) | state->l;
            cmp(state, state->memory[address]);
            state->t_states += 7;
            break;
        }    
        case 0xbf: cmp(state, state->a); state->t_states += 4; break;    // CMP A
        case 0xc0: {                    // RNZ
            if (!state->cc.z) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }   
        case 0xc1: pop(state, &state->b, &state->c); break;    // POP B
        case 0xc2: {                 // JNZ adr
            state->t_states += 10;

            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }

            state->pc += 2;
            break;
        }   
        case 0xc3: {                  // JMP adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->pc = address;
            state->t_states += 10;
            return false;
        }    
        case 0xc4: {                  // CNZ adr
            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }
            
            state->pc += 2;
            state->t_states += 11;
            break;
        }    
        case 0xc5: push(state, state->b, state->c); break;    // PUSH B
        case 0xc6: {                 // ADI D8
            uint16_t result = (uint16_t) state->a + (uint16_t) opcode[1];
            state->cc.z = (result & 0xff) == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = result > 0xff;
            state->cc.p = parity((uint8_t) result);
            state->a = result & 0xff;
            state->pc += 1;
            state->t_states += 7;
            break; 
        }
        case 0xc7: call(state, 0x00); state->t_states += 11; return false;           // 	RST 0
        case 0xc8:{             // RZ
            if (state->cc.z) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }    
        case 0xc9: ret(state); state->t_states += 10; return false;   
        case 0xca: {                 // JZ adr
            state->t_states += 10;

            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }

            state->pc += 2;
            break; 
        }   
        case 0xcb: break;
        case 0xcc: {                  // CZ adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            } 

            state->pc += 2;
            state->t_states += 11;
            break; 
        }   
        case 0xcd: {                  // CALL adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            call(state, address);
            state->t_states += 17;
            return false;
        }
        case 0xce: {                 // ACI D8
            uint16_t result = (uint16_t) state->a + (uint16_t) opcode[1] + (uint16_t) state->cc.cy;
            state->cc.z = (result & 0xff) == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = result > 0xff;
            state->cc.p = parity(result & 0xff);
            state->pc += 1;
            state->a = result & 0xff;
            state->t_states += 7;
            break;  
        }  
        case 0xcf: call(state, 0x8); state->t_states += 11; return false;    // RST 1
        case 0xd0: {                // RNC
            if (!state->cc.cy) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }    
        case 0xd1: pop(state, &state->d, &state->e); break;    // POP D
        case 0xd2: {                 // 	JNC adr
            state->t_states += 10;

            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }
            
            state->pc += 2;
            break;
        }       
        case 0xd3: {                // OUT D8
            state->t_states += 10;

            if (io == NULL) {
                state->pc += 2;
                return false;
            }

            io->machine_out(state, opcode[1]);
            state->pc +=1; 
            break;
        }               
        case 0xd4: {                 // CNC adr
            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }
            
            state->pc += 2;
            state->t_states += 11;
            break;  
        }  
        case 0xd5: push(state, state->d, state->e); break;              // PUSH D    
        case 0xd6: {                 // SUI D8
            uint16_t result = (uint16_t) state->a - (uint16_t) opcode[1];
            state->cc.z = (result & 0xff) == 0;    
            state->cc.s = (result & 0x80) != 0;    
            state->cc.cy = opcode[1] > state->a;
            state->cc.ac = ((state->a & 0x0f) - (opcode[1] & 0x0f)) < 0;
            state->cc.p = parity(result & 0xff);
            state->a = result & 0xff;
            state->pc += 1;
            state->t_states += 7;
            break;
        }
        case 0xd7: call(state, 0x10); state->t_states += 11; return false;    // RST 2
        case 0xd8: {                // RC
            if (state->cc.cy) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }   
        case 0xd9: break;    
        case 0xda: {                 // JC adr
            state->t_states += 10;

            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }

            state->pc += 2;
            break; 
        }  
        case 0xdb: {                // IN D8
            state->t_states += 10;

            if (io == NULL) {
                state->pc += 2;
                return false;
            }

            state->a = io->machine_in(state, opcode[1], keyboard_state);
            state->pc += 1;
            break; 
        }
        case 0xdc: {                 // CC adr
            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }

            state->pc += 2;
            state->t_states += 11;
            break;  
        }  
        case 0xdd: break;    
        case 0xde: {                 // SBI D8 
            uint16_t result = (uint16_t) state->a - (uint16_t) opcode[1] - (uint16_t) state->cc.cy;
            state->cc.z = (result & 0xff) == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = opcode[1] + state->cc.cy > state->a;
            state->cc.ac = ((state->a & 0x0f) - (opcode[1] & 0x0f) - state->cc.cy) < 0;
            state->cc.p = parity(result & 0xff);
            state->a = result & 0xff;
            state->pc += 1;
            state->t_states += 7;
            break;
        }
        case 0xdf: call(state, 0x18); state->t_states += 11; return false;    // RST 3
        case 0xe0:{             // RPO
            if (!state->cc.p) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }   
        case 0xe1: pop(state, &state->h, &state->l); break;    // POP H
        case 0xe2: {                 // JPO adr
            state->t_states += 10;

            if(!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }
            
            state->pc += 2;
            break;
        }    
        case 0xe3: {                 // XTHL
            uint8_t temp = state->l;
            state->l = state->memory[state->sp];
            state->memory[state->sp] = temp;

            temp = state->h;
            state->h = state->memory[state->sp + 1];
            state->memory[state->sp + 1] = temp;
            state->t_states += 18;
            break;  
        }  
        case 0xe4: {                 // CPO adr
            if (!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }

            state->pc += 2;
            state->t_states += 11;
            break;  
        }  
        case 0xe5: push(state, state->h, state->l); break;    // PUSH H
        case 0xe6: {                 // ANI D8
            uint8_t result = state->a & (uint8_t) opcode[1];
            state->cc.z = result == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = 0;
            state->cc.p = parity(result);
            state->a = result;
            state->pc += 1;
            state->t_states += 7;
            break;
        }
        case 0xe7: call(state, 0x20); state->t_states += 11; return false;               // 	RST 4
        case 0xe8: {                // RPE
            if (state->cc.p) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }     
        case 0xe9: {                 // PCHL
            uint16_t hl = (state->h << 8) | state->l;
            state->pc = hl;
            state->t_states += 5;
            break; 
        }   
        case 0xea: {                 // JPE adr
            state->t_states += 10;

            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }
            
            state->pc += 2;
            break;  
        }  
        case 0xeb: {                 // XCHG
            uint8_t temp = state->h;
            state->h = state->d;
            state->d = temp;

            temp = state->l;
            state->l = state->e;
            state->e = temp;
            state->t_states += 4;
            break; 
        }   
        case 0xec: {                 // CPE adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }

            state->pc += 2;
            state->t_states += 11;
            break; 
        }   
        case 0xed: break;    
        case 0xee: {                 // XRI D8
            uint8_t result = state->a ^ opcode[1];
            state->cc.z = result == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = 0;
            state->cc.p = parity(result);
            state->a = result;
            state->pc += 1;
            state->t_states += 7;
            break; 
        }   
        case 0xef: call(state, 0x28); state->t_states += 11; return false;    // RST 5
        case 0xf0: {                // RP
            if (state->cc.p) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }    
        case 0xf1: {                 // POP PSW
            uint8_t psw = state->memory[state->sp];
            state->cc.z = (psw & 0x40) != 0;
            state->cc.s = (psw & 0x80) != 0;
            state->cc.p = (psw & 0x04) != 0;
            state->cc.cy = (psw & 0x01) != 0;
            state->cc.ac = (psw & 0x10) != 0;
            state->a = state->memory[state->sp + 1];
            state->sp += 2;
            state->t_states += 10;
            break; 
        }
        case 0xf2: {                 // JP adr
            state->t_states += 10;

            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            } 
            
            state->pc += 2;
            break; 
        }   
        case 0xf3: state->interrupt_enable = false; state->t_states += 4; break;                // DI    
        case 0xf4: {                 // CP adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }

            state->pc += 2;
            state->t_states += 11;
            break; 
        }   
        case 0xf5: {                 // PUSH PSW
            uint16_t psw = (state->a << 8) | 
                            ((state->cc.s & 0x80) |
                            (state->cc.z & 0x40) |
                            (state->cc.ac & 0x10) |
                            (state->cc.p & 0x04) |
                            (state->cc.cy & 0x01));

            state->memory[state->sp - 1] = (psw >> 8) & 0xff;
            state->memory[state->sp - 2] = psw & 0xff;
            state->sp -= 2;
            state->t_states += 11;
            break;
        }
        case 0xf6: {                // // ORI D8
            uint8_t result = state->a | (uint8_t) opcode[1];
            state->a = result;
            state->cc.z = result == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.p = parity(result);
            state->cc.ac = (result & 0x0F) == 0x00; 
            state->pc += 1;
            state->t_states += 7;
            break; 
        }    
        case 0xf7: call(state, 0x30); state->t_states += 11; return false;    // RST 6
        case 0xf8: {                // RM
            if (state->cc.s) {
                ret(state);
                state->t_states += 11;
                return false;
            }

            state->t_states += 5;
            break;
        }   
        case 0xf9: {                 // SPHL
            uint16_t address = (state->h << 8) | state->l;
            state->sp = address;
            state->t_states += 5;
            break; 
        }   
        case 0xfa: {                 // JM adr
            state->t_states += 10;

            if (state->cc.s) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
                return false;
            }
            
            state->pc += 2;
            break; 
        }   
        case 0xfb: state->interrupt_enable = true; state->t_states += 4; break;             // EI
        case 0xfc: {                 // 	CM adr
            if (state->cc.s) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
                state->t_states += 17;
                return false;
            }

            state->pc += 2;
            state->t_states += 11;
            break;
        }    
        case 0xfd: break;    
        case 0xfe: {                 // 	CPI D8
            cmp(state, opcode[1]);
            state->pc += 1;
            state->t_states += 7;
            break; 
        }   
        case 0xff: call(state, 0x38); state->t_states += 11; return false;    // 	RST 7
    } 

    state->pc += 1;
    return false;
}

void call(State8080* state, uint16_t address) {
    uint8_t pc_hi = (state->pc >> 8) & 0xff;
    uint8_t pc_lo = state->pc & 0xff;
    
    push(state, pc_hi, pc_lo);
    state->pc = address;
}

void ret(State8080* state) {
    uint8_t pc_lo = state->memory[state->sp];
    uint8_t pc_hi = state->memory[state->sp + 1];

    state->pc = (pc_hi << 8) | pc_lo;
    state->pc += 3;
    state->sp += 2;
}

void push(State8080* state, uint8_t reg_hi, uint8_t reg_lo) {
    state->memory[state->sp - 2] = reg_lo;
    state->memory[state->sp - 1] = reg_hi;
    state->sp -= 2;
    state->t_states += 11;
}

void pop(State8080* state, uint8_t* reg_hi, uint8_t* reg_lo) {
    *reg_lo = state->memory[state->sp];
    *reg_hi = state->memory[state->sp + 1];
    state->sp += 2;
    state->t_states += 10;
}

int parity(uint8_t num) {
    int count = 0;
    
    while (num) {
        count += num & 1;
        num = num >> 1;
    }

    return count % 2 == 0;
}

void add_to_accumulator(State8080* state, uint8_t num) {
    uint16_t result = (uint16_t) state->a + (uint16_t) num;
    
    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = result > 0xff;
    state->cc.p = parity(result & 0xff);
    
    state->a = result &0xff;
}

void adc(State8080* state, uint8_t num) {
    uint16_t result = (uint16_t) state->a + (uint16_t) num + (uint16_t) state->cc.cy;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = result > 0xff;
    state->cc.p = parity(result & 0xff);
    
    state->a = result & 0xff;
}

void subtract_to_accumulator(State8080* state, uint8_t num) {
    uint16_t result = (uint16_t) state->a - (uint16_t) num;
    
    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = state->a < num;
    state->cc.p = parity(result & 0xff);
    
    state->a = result & 0xff;
}

void sbb(State8080* state, uint8_t num) {
    uint16_t result = (uint16_t) state->a - (uint16_t) num - (uint16_t) state->cc.cy;
    
    state->cc.z = ((result & 0xff) == 0);
    state->cc.s = ((result & 0x80) != 0);
    state->cc.cy = (state->a < num + state->cc.cy);
    state->cc.p = parity(result & 0xff);
    
    state->a = result & 0xff;
}

void ana(State8080* state, uint8_t num) {
    uint8_t result = state->a & num;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = 0;
    state->cc.p = parity(result);
    state->cc.ac = ((state->a | num) & 0x08) != 0;

    state->a = result;
}

void xra(State8080* state, uint8_t num) {
    uint8_t result = state->a ^ num;

    state->cc.z = result == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = 0;
    state->cc.p = parity(result);

    state->a = result;
}

void ora(State8080* state, uint8_t num) {
    uint8_t result = state->a | num;

    state->cc.z = result == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = 0;
    state->cc.p = parity(result);

    state->a = result;
}

void cmp(State8080* state, uint8_t num) {
    uint16_t result = (uint16_t) state->a - (uint16_t) num;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = state->a < num;
    state->cc.ac = ((state->a & 0x0F) < (num & 0x0F));
    state->cc.p = parity(result & 0xff);
}

void inr(State8080* state, uint8_t* reg) {
    uint16_t result = (uint16_t) *reg + (uint16_t) 1;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.p = parity(result & 0xff);
    state->cc.ac = ((*reg & 0x0F) + 1) > 0x0F;

    *reg = result & 0xff;
}

void dcr(State8080* state, uint8_t* reg) {
    uint8_t original = *reg;
    uint8_t result = original - 1;

    state->cc.z = result == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.p = parity(result);
    state->cc.ac = (original & 0x0F) == 0x00;       

    *reg = result;                                
}

void dad(State8080* state, uint16_t num) {
    uint16_t hl = (state->h << 8) | state->l;
    hl += num;

    state->cc.cy = (hl & 0x10000) != 0;

    state->h = (hl >> 8) & 0xff;
    state->l = hl & 0xff;
}

void generate_interrupt(State8080* state, int interrupt_num) {
    if (state != NULL && interrupt_num >= 0 && interrupt_num <= 7) {
        push(state, (state->pc >> 8) & 0xff, (state->pc & 0xff));
        state->pc = 8 * interrupt_num;
        state->interrupt_enable = false;
    }
}

State8080* init_8080_state(void) {
    State8080* state = NULL;
    
	state = (State8080*) calloc(1, sizeof(State8080));
    if (state == NULL) {
        printf("Error allocating memory for state.\n");
        exit(1);
    }

    state->t_states = 0;

	state->memory = (uint8_t*) malloc(16 * 0x1000);
    if (state->memory == NULL) {
        printf("Error allocating memory for memory buffer.\n");
        free(state);
        exit(1);
    }

    memset(state->memory, 0x0, 0x10000);
	return state;
}