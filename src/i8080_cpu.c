#include <stdio.h>
#include <stdlib.h>
#include "i8080_cpu.h"

// https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf

void unimplemented_instruction(State8080* state) {    
    state->pc -= 1;

    printf ("Error: Unimplemented instruction\n");    
    exit(1);    
}

void emulate_i8080(State8080* state) {    
    unsigned char *opcode = &state->memory[state->pc];    

    switch(*opcode) {    
        case 0x00: break;           // NOP    
        case 0x01: {                  // LXI B,D16
            state->c = opcode[1];
            state->b = opcode[2];
            state->pc += 2;
            break;
        }  
        case 0x02: {                  // STAX B
            state->b = opcode[1];
            state->c = opcode[2];
            break;
        }   
        case 0x03: {                  // INX B
            uint16_t bc = (state->b << 8) | state->c;
            bc += 1;
            state->b = (bc >> 8) & 0xff;
            state->c = bc & 0xff;
            break; 
        }  
        case 0x04: {                  // INR B
            inr(state, &state->b);
            break;
        }  
        case 0x05: {                  // DCR B
            dcr(state, &state->b);
            break;
        }  
        case 0x06: {                  // MVI B, D8
            state->b = opcode[1];
            state->pc += 1;
            break;
        }   
        case 0x07: {                  // RLC
            state->cc.cy = (state->a & 0x80) != 0;
            state->a = (state->a << 1) | (state->a >> 7);
            break;
        }
        case 0x08:  break;    
        case 0x09: {                  // DAD B
            uint16_t bc = (state->b << 8) | state->c;
            dad(state, bc);
            break;
        }  
        case 0x0a: {                  // LDAX B 
            uint16_t address = (state->b << 8) | state->c;
            state->a = state->memory[address];
            break;
        }   
        case 0x0b: {                  // DCX B
            uint16_t bc = (state->b << 8) | state->c;
            bc -= 1;
            state->b = (state->b >> 8) & 0xff;
            state->c = bc & 0xff;
            break;    
        }
        case 0x0c: {                  // INR C
            inr(state, &state->c);
            break;
        }    
        case 0x0d: {                  // DCR C
            dcr(state, &state->c);
            break;
        }   
        case 0x0e: {                  // MVI C,D8 
            state->c = opcode[1];
            state->pc += 1;
            break;
        }   
        case 0x0f: {                  // RRC
            uint8_t lsb = state->a & 0x01;
            state->cc.cy = lsb;
            state->a = (state->a >> 1) | (lsb << 7);
            break;
        }
        case 0x10: break;    
        case 0x11: {                  // LXI D,D16
            state->d = opcode[2];
            state->e = opcode[1];
            state->pc += 2;
            break; 
        }   
        case 0x12: {                  // STAX D
            uint16_t address = (state->d << 8) | state->e;
            state->memory[address] = state->a;
            break;
        }
        case 0x13: {                  // INX D
            uint16_t de = (state->d << 8) | state->e;
            de += 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            break;
        }    
        case 0x14: {                  // INR D
            inr(state, &state->d);
            break;
        }    
        case 0x15: {                  // DCR D
            dcr(state, &state->d);
            break;
        }    
        case 0x16: {                  // MVI D, D8
            state->d = opcode[1];
            state->pc += 1;
            break;
        }    
        case 0x17: {                  // RAL
            uint8_t msb = (state->a & 0x80) != 0;
            state->a = (state->a << 1) | (state->cc.cy << 7);
            state->cc.cy = msb;
            break;
        }   
        case 0x18: break;    
        case 0x19: {                  // DAD D
            uint16_t de = (state->d << 8) | state->e;
            dad(state, de);
            break;
        }   
        case 0x1a: {                  // LDAX D 
            uint16_t address = (state->d << 8) | state->e;
            state->a = state->memory[address];
            break;
        }
        case 0x1b: {                  // DCX D
            uint16_t de = (state->d << 8) | state->e;
            de -= 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            break;
        }    
        case 0x1c: {                  // INR E
            inr(state, &state->e);
            break;
        }    
        case 0x1d: {                  // DCR E
            dcr(state, &state->e);
            break; 
        }   
        case 0x1e: {                  // MVI E,D8 
            state->e = opcode[1];
            state->pc += 1;
            break;
        }    
        case 0x1f: {                  // RAR
            state->cc.cy = state->a & 0x01;
            state->a = (state->a >> 1) | (state->cc.cy << 7);
            break;
        }
        case 0x20: break;    
        case 0x21: {                  // LXI H,D16
            state->h = opcode[2];
            state->l = opcode[1];
            state->pc += 2;
            break;
        }    
        case 0x22: {                  // SHLD adr
            uint16_t address = (opcode[1] << 8) | opcode[1];
            state->memory[address] = state->l;
            state->memory[address + 1] = state->h;
            state->pc += 2;
            break;
        }   
        case 0x23: {                  // INX H
            uint16_t hl = (state->h << 8) | state->l;
            hl += 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            break;
        }    
        case 0x24: {                  // INR H
            inr(state, &state->h);
            break;
        }    
        case 0x25: {                  // DCR H
            dcr(state, &state->h);
            break;
        }    
        case 0x26: {                  // MVI H,D8 
            state->h = opcode[1];
            state->pc += 1;
            break;
        }    
        case 0x27: break;    
        case 0x28: break;    
        case 0x29: {                  // DAD H
            uint16_t hl = (state->h << 8) | state->l;
            dad(state, hl);
            break;
        }    
        case 0x2a: {                  // LHLD adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->l = state->memory[address];
            state->h = state->memory[address + 1];
            state->pc += 2;
            break;
        }    
        case 0x2b: {                  // CX H
            uint16_t hl = (state->h << 8) | state->l;
            hl -= 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            break;
        }   
        case 0x2c: {                  // INR L
            inr(state, &state->l);
            break; 
        }   
        case 0x2d: {                  // DCR L
            dcr(state, &state->l);
            break;
        }    
        case 0x2e: {                  // MVI L, D8 
            state->l = opcode[1];
            state->pc += 1;
            break;
        }    
        case 0x2f: {                  // CMA
            state->a = ~state->a;
            break;
        }  
        case 0x30: break;    
        case 0x31: {                  // LXI SP, D16
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->sp = address;
            state->pc += 2;
            break;
        }  
        case 0x32: {                  // STA adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->memory[address] = state->a;
            state->pc += 2;
            break;
        }    
        case 0x33: {                  // INX SP
            state->sp += 1;
            break;
        }    
        case 0x34: {                  // INR M
            uint16_t address = (state->h << 8) | state->l;
            inr(state, &state->memory[address]);
            break;
        }    
        case 0x35: {                  // DCR M
            uint16_t address = (state->h << 8) | state->l;
            dcr(state, &state->memory[address]);
            break;
        }    
        case 0x36: {                  // MVI M,D8
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = opcode[1];
            state->pc += 1;
            break;
        }
        case 0x37: {                  // STC
            state->cc.cy = 1;
            break;
        }    
        case 0x38: break;    
        case 0x39: {                  // DAD SP
            dad(state, state->sp);
            break;
        }   
        case 0x3a: {                  // LDA adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->a = state->memory[address];
            state->pc += 2;
            break;
        }    
        case 0x3b: {                  // DCX SP
            state->sp -= 1;
            break;
        }    
        case 0x3c: {                  // INR A
            inr(state, &state->a);
            break;
        }    
        case 0x3d: {                  // DCR A
            dcr(state, &state->a);
            break;
        }  
        case 0x3e: {                  // MVI A,D8
            state->a = opcode[1];
            state->pc += 1;
            break;
        }    
        case 0x3f: {                  // CMC
            state->cc.cy = ~state->cc.cy;
            break;
        }    
        case 0x40: {                  // MOV B,B
            state->b = state->b;
            break;
        }    
        case 0x41: {                  // MOV B,C
            state->b = state->c;
            break;
        }    
        case 0x42: {                 // MOV B,D
            state->b = state->d;
            break; 
        }   
        case 0x43: {                  // MOV B,E
            state->b = state->e;
            break;
        }    
        case 0x44: {                  // MOV B,H
            state->b = state->h;
            break;
        }    
        case 0x45: {                  // MOV B,L
            state->b = state->l;
            break;
        }    
        case 0x46: {                  // MOV B,M
            uint16_t address = (state->h << 8) | state->l;
            state->b = state->memory[address];
            break;
        }    
        case 0x47: {                  // MOV B,A
            state->b = state->a;
            break;
        }    
        case 0x48: {                  // MOV C,B
            state->c = state->b;
            break;
        }    
        case 0x49: {                  // MOV C,C
            state->c = state->c;
            break; 
        }   
        case 0x4a: {                 // MOV C,D
            state->c = state->d;
            break;  
        }  
        case 0x4b: {                  // MOV C,E
            state->c = state->e;
            break; 
        }   
        case 0x4c: {                 // MOV C,H
            state->c = state->h;
            break;  
        }  
        case 0x4d: {                  // MOV C,L
            state->c = state->l;
            break;
        }    
        case 0x4e: {                  // MOV C,M
            uint16_t address = (state->h << 8) | state->l;
            state->c = state->memory[address];
            break; 
        }   
        case 0x4f: {                  // MOV C,A
            state->c = state->a;
            break; 
        }   
        case 0x50: {                  // MOV D,B
            state->d = state->b;
            break;
        }    
        case 0x51: {                  // MOV D,C
            state->d = state->c;
            break;
        }    
        case 0x52: {                  // MOV D,D 
            state->d = state->d;
            break; 
        }   
        case 0x53: {                  // MOV D,E
            state->d = state->e;
            break;
        }    
        case 0x54: {                  // MOV D,H
            state->d = state->h;
            break;
        }    
        case 0x55: {                  // MOV D,L
            state->d = state->l;
            break;
        }    
        case 0x56: {                  // MOV D,M
            uint16_t address = (state->h << 8) | state->l;
            state->d = state->memory[address];
            break;
        }    
        case 0x57: {                  // MOV D,A
            state->d = state->a;
            break;
        }    
        case 0x58: {                  // MOV E,B
            state->e = state->b;
            break;
        }    
        case 0x59: {                  // MOV E,C
            state->e = state->c;
            break;
        }    
        case 0x5a: {                  // MOV E,D
            state->e = state->d;
            break;
        }    
        case 0x5b: {                  // MOV E,E
            state->e = state->e;
            break;
        }    
        case 0x5c: {                  // MOV E,H
            state->e = state->h;
            break; 
        }   
        case 0x5d: {                  // MOV E,L
            state->e = state->l;
            break;
        }    
        case 0x5e: {                  // MOV E,M
            uint16_t address = (state->h << 8) | state->l;
            state->e = state->memory[address];
            break;
        }    
        case 0x5f: {                  // MOV E,A
            state->e = state->a;
            break;
        }   
        case 0x60: {                  // MOV H,B
            state->h = state->b;
            break; 
        }   
        case 0x61: {                 // MOV H,C
            state->h = state->c;
            break; 
        }   
        case 0x62: {                 // MOV H,D
            state->h = state->b;
            break;
        }    
        case 0x63: {                 // MOV H,E
            state->h = state->e;
            break;
        }    
        case 0x64: {                 // MOV H,H
            state->h = state->h;
            break; 
        }   
        case 0x65: {                 // MOV H,L
            state->h = state->l;
            break; 
        }   
        case 0x66: {                 // MOV H,M
            uint16_t address = (state->h << 8) | state->l;
            state->h = state->memory[address];
            break;  
        }  
        case 0x67: {                // MOV H,A
            state->h = state->a;
            break;  
        }  
        case 0x68: {                 // MOV L,B
            state->l = state->b;
            break;
        }    
        case 0x69: {                 // MOV L,C
            state->l = state->c;
            break;
        }    
        case 0x6a: {                 // MOV L,D
            state->l = state->d;
            break;
        }    
        case 0x6b: {                 // MOV L,E
            state->l = state->e;
            break;
        }    
        case 0x6c: {                 // MOV L,H
            state->l = state->h;
            break;
        }    
        case 0x6d: {                 // MOV L,L
            state->l = state->l;
            break;
        }    
        case 0x6e: {                 // MOV L,M
            uint16_t address = (state->h << 8) | state->l;
            state->l = state->memory[address];
            break;
        }    
        case 0x6f: {                 // MOV L,A
            state->l = state->a;
            break; 
        }   
        case 0x70: {                  // MOV M,B
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->b;
            break;
        }    
        case 0x71: {                 // MOV M,C
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->c;
            break;
        }    
        case 0x72: {                 // MOV M,D
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->d;
            break;
        }    
        case 0x73: {                 // MOV M,E
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->e;
            break;
        }    
        case 0x74: {                 // MOV M,H
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->h;
            break;
        }    
        case 0x75: {                 // MOV M,L
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->l;
            break;
        }    
        case 0x76: exit(0); break;   // HLT
        case 0x77: {                 // MOV M,L
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->l;
            break;
        }    
        case 0x78: state->a = state->b; break;    // MOV A,B
        case 0x79: state->a = state->c; break;    // MOV A,C
        case 0x7a: state->a = state->d; break;    // MOV A,D
        case 0x7b: state->a = state->e; break;    // MOV A,E
        case 0x7c: state->a = state->h; break;    // MOV A,H
        case 0x7d: state->a = state->l; break;    // MOV A,L
        case 0x7e: {                 // MOV A,M
            uint16_t address = (state->h << 8) | state->l;
            state->a = state->memory[address];
            break;
        }    
        case 0x7f: state->a = state->a; break;    // MOV A,A  
        case 0x80: add_to_accumulator(state, state->b); break;    // ADD B 
        case 0x81: add_to_accumulator(state, state->c); break;    // ADD C
        case 0x82: add_to_accumulator(state, state->d); break;    // ADD D
        case 0x83: add_to_accumulator(state, state->e); break;    // ADD E
        case 0x84: add_to_accumulator(state, state->h); break;    // ADD H
        case 0x85: add_to_accumulator(state, state->l); break;    // ADD L
        case 0x86: {                 // 	ADD M
            uint16_t address = (state->h << 8) | state->l;
            add_to_accumulator(state, state->memory[address]);
            break; 
        }   
        case 0x87: add_to_accumulator(state, state->a); break;    // ADD A
        case 0x88: adc(state, state->b); break;    // ADC B
        case 0x89: adc(state, state->c); break;    // ADC C
        case 0x8a: adc(state, state->d); break;    // ADC D
        case 0x8b: adc(state, state->e); break;    // ADC E
        case 0x8c: adc(state, state->h); break;    // ADC H
        case 0x8d: adc(state, state->l); break;    // ADC L
        case 0x8e: {                 // ADC M
            uint16_t address = (state->h << 8) | state->l;
            adc(state, state->memory[address]);
            break;
        }    
        case 0x8f: adc(state, state->a); break;    // ADC A  
        case 0x90: subtract_to_accumulator(state, state->b); break;    // SUB B
        case 0x91: subtract_to_accumulator(state, state->c); break;    // SUB C
        case 0x92: subtract_to_accumulator(state, state->d); break;    // SUB D
        case 0x93: subtract_to_accumulator(state, state->d); break;    // SUB E
        case 0x94: subtract_to_accumulator(state, state->h); break;    // SUB H
        case 0x95: subtract_to_accumulator(state, state->l); break;    // SUB L
        case 0x96: {                 // SUB M
            uint16_t address = (state->h << 8) | state->l;
            subtract_to_accumulator(state, state->memory[address]);
            break; 
        }   
        case 0x97: subtract_to_accumulator(state, state->a); break;    // SUB A
        case 0x98: sbb(state, state->b); break;    // SBB B
        case 0x99: sbb(state, state->c); break;    // SBB C
        case 0x9a: sbb(state, state->d); break;    // SBB D
        case 0x9b: sbb(state, state->e); break;    // SBB E
        case 0x9c: sbb(state, state->h); break;    // SBB H
        case 0x9d: sbb(state, state->l); break;    // SBB L
        case 0x9e: {                 // SBB M
            uint16_t address = (state->h << 8) | state->l;
            sbb(state, state->memory[address]);
            break;   
        } 
        case 0x9f: sbb(state, state->a); break;    // SBB A
        case 0xa0: ana(state, state->b); break;    // ANA B
        case 0xa1: ana(state, state->c); break;    // ANA C
        case 0xa2: ana(state, state->d); break;    // ANA D
        case 0xa3: ana(state, state->e); break;    // ANA E
        case 0xa4: ana(state, state->h); break;    // ANA H
        case 0xa5: ana(state, state->l); break;    // ANA L
        case 0xa6: {                 // ANA M
            uint16_t address = (state->h << 8) | state->l;
            ana(state, state->memory[address]);
            break;  
        }  
        case 0xa7: ana(state, state->a); break;    // ANA A
        case 0xa8: xra(state, state->b); break;    // XRA B
        case 0xa9: xra(state, state->c); break;    // XRA C
        case 0xaa: xra(state, state->d); break;    // XRA D
        case 0xab: xra(state, state->e); break;    // XRA E
        case 0xac: xra(state, state->h); break;    // XRA H
        case 0xad: xra(state, state->l); break;    // XRA L
        case 0xae: {                  // XRA M
            uint16_t address = (state->h << 8) | state->l;
            xra(state, state->memory[address]);
            break;
        }    
        case 0xaf: xra(state, state->a); break;    // XRA A
        case 0xb0: ora(state, state->b); break;    // ORA B
        case 0xb1: ora(state, state->c); break;    // ORA C
        case 0xb2: ora(state, state->d); break;    // ORA D
        case 0xb3: ora(state, state->e); break;    // ORA E
        case 0xb4: ora(state, state->h); break;    // ORA H
        case 0xb5: ora(state, state->l); break;    // ORA L
        case 0xb6: {                 // ORA M
            uint16_t address = (state->h << 8) | state->l;
            ora(state, state->memory[address]);
            break;  
        }  
        case 0xb7: ora(state, state->a); break;    // ORA A
        case 0xb8: cmp(state, state->b); break;    // CMP B
        case 0xb9: cmp(state, state->c); break;    // CMP C
        case 0xba: cmp(state, state->d); break;    // CMP D
        case 0xbb: cmp(state, state->e); break;    // CMP E
        case 0xbc: cmp(state, state->h); break;    // CMP H
        case 0xbd: cmp(state, state->l); break;    // CMP L
        case 0xbe: {                 // CMP M
            uint16_t address = (state->h << 8) | state->l;
            cmp(state, state->memory[address]);
            break;
        }    
        case 0xbf: cmp(state, state->a); break;    // CMP A
        case 0xc0: if (!state->cc.z) ret(state); break;   // RNZ
        case 0xc1: pop(state, &state->b, &state->c); break;    // POP B
        case 0xc2: {                 // JNZ adr
            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address - 1;
            } else {
                state->pc += 2;
            }
            break;
        }   
        case 0xc3: {                  // JMP adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->pc = address - 1;
            break;
        }    
        case 0xc4: {                  // CNZ adr
            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;
        }    
        case 0xc5: push(state, state->b, state->c); break;    // PUSH B
        case 0xc6: {                 // ADI D8
            uint16_t result = (uint16_t) state->a + (uint16_t) opcode[1];
            state->cc.z = (result & 0xff) == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = result > 0xff;
            state->cc.p = parity(result & 0xff);
            state->a = result & 0xff;
            state->pc += 1;
            break; 
        }   
        case 0xc7: call(state, 0x00); break;    // 	RST 0
        case 0xc8: if (state->cc.z) ret(state); break;    // RZ
        case 0xc9: ret(state); break;    // RET
        case 0xca: {                 // JZ adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break; 
        }   
        case 0xcb: break;
        case 0xcc: {                  // CZ adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break; 
        }   
        case 0xcd: {                  // CALL adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            call(state, address);
            break;
        }
        case 0xce: {                 // ACI D8
            uint16_t result = (uint16_t) state->a + (uint16_t) opcode[1] + (uint16_t) state->cc.cy;
            state->cc.z = (result & 0xff) == 0;
            state->cc.s = (result & 0x80) != 0;
            state->cc.cy = result > 0xff;
            state->cc.p = parity(result & 0xff);
            state->pc += 1;
            state->a = result & 0xff;
            break;  
        }  
        case 0xcf: call(state, 0x8); break;    // RST 1
        case 0xd0: if (!state->cc.cy) ret(state); break;    // RNC
        case 0xd1: pop(state, &state->d, &state->e); break;    // POP D
        case 0xd2: {                 // 	JNC adr
            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }      
            break;
        }    
        case 0xd3: break;    
        case 0xd4: {                 // CNC adr
            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;  
        }  
        case 0xd5:                  // PUSH D
            push(state, state->d, state->e);
            break;    
        case 0xd6: {                 // SUI D8
            uint16_t result = (uint16_t) state->a - (uint16_t) opcode[1];
            state->cc.z = (result & 0xff) == 0;    
            state->cc.s = (result & 0x80) != 0;    
            state->cc.cy = opcode[1] > state->a;
            state->cc.ac = ((state->a & 0x0f) - (opcode[1] & 0x0f)) < 0;
            state->cc.p = parity(result & 0xff);
            state->a = result & 0xff;
            state->pc += 1;
            break;
        }
        case 0xd7: call(state, 0x10); break;    // RST 2
        case 0xd8: if (state->cc.cy) ret(state); break;    // RC
        case 0xd9: break;    
        case 0xda: {                 // JC adr
            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break; 
        }  
        case 0xdb: break;    
        case 0xdc: {                 // CC adr
            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
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
            break;
        }
        case 0xdf: call(state, 0x18); break;    // RST 3
        case 0xe0: if (!state->cc.p) ret(state); break;    // RPO
        case 0xe1: pop(state, &state->h, &state->l); break;    // POP H
        case 0xe2: {                 // JPO adr
            if(!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;
        }    
        case 0xe3: {                 // XTHL
            uint8_t temp = state->l;
            state->l = state->memory[state->sp];
            state->memory[state->sp] = temp;

            temp = state->h;
            state->h = state->memory[state->sp + 1];
            state->memory[state->sp + 1] = temp;
            break;  
        }  
        case 0xe4: {                 // CPO adr
            if (!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
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
            state->pc += 2;
            break;
        }
        case 0xe7: call(state, 0x20); break;    
        case 0xe8: if (state->cc.p) ret(state); break;    // RPE
        case 0xe9: {                 // PCHL
            uint16_t hl = (state->h << 8) | state->l;
            state->pc = hl;
            break; 
        }   
        case 0xea: {                 // JPE adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;  
        }  
        case 0xeb: {                 // XCHG
            uint8_t temp = state->h;
            state->h = state->d;
            state->d = temp;

            temp = state->l;
            state->l = state->e;
            state->e = temp;
            break; 
        }   
        case 0xec: {                 // CPE adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
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
            state->pc += 2;
            break; 
        }   
        case 0xef: call(state, 0x28); break;    // RST 5
        case 0xf0: if (!state->cc.z) ret(state); break;    // RP
    case 0xf1: {                 // POP PSW
        uint8_t psw = state->memory[state->sp];
        state->cc.z = (psw & 0x40) != 0;
        state->cc.s = (psw & 0x80) != 0;
        state->cc.p = (psw & 0x04) != 0;
        state->cc.cy = (psw & 0x01) != 0;
        state->cc.ac = (psw & 0x10) != 0;
        state->a = state->memory[state->sp + 1];
        state->sp += 2;
        break; 
    }
    case 0xf2: {                 // JP adr
        if (state->cc.z) {
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->pc = address;
        } else {
            state->pc += 2;
        }
        break; 
    }   
    case 0xf3: break;    
    case 0xf4: {                 // CP adr
        if (state->cc.p) {
            uint16_t address = (opcode[2] << 8) | opcode[1];
            call(state, address);
        } else {
            state->pc += 2;
        }
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
        break;
    }
    case 0xf6: state->a = state->a | opcode[1]; break;    // ORI D8
    case 0xf7: call(state, 0x30); break;    // RST 6
    case 0xf8: if (state->cc.s) ret(state); break;    // RM
    case 0xf9: {                 // SPHL
        uint16_t address = (state->h << 8) | state->l;
        state->sp = address;
        break; 
    }   
    case 0xfa: {                 // JM adr
        if (state->cc.s) {
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->pc = address;
        } else {
            state->pc += 2;
        }
        break; 
    }   
    case 0xfb: break;    
    case 0xfc: {                 // 	CM adr
        if (state->cc.s) {
            uint16_t address = (opcode[2] << 8) | opcode[1];
            call(state, address);
        } else {
            state->pc += 2;
        }
        break;
    }    
    case 0xfd: break;    
    case 0xfe: {                 // 	CPI D8
        cmp(state, opcode[1]);
        state->pc += 1;
        break; 
    }   
    case 0xff: call(state, 0x38); break;    // 	RST 7
    }    
    state->pc += 1;
}

void call(State8080* state, uint16_t address) {
    uint8_t pc_hi = (state->pc >> 8) & 0xff;
    uint8_t pc_lo = state->pc & 0xff;
    
    state->memory[state->sp - 1] = pc_hi;
    state->memory[state->sp - 2] = pc_lo;
    state->sp -= 2;
    state->pc = address - 1;
}

void ret(State8080* state) {
    uint8_t pc_lo = state->memory[state->sp];
    uint8_t pc_hi = state->memory[state->sp + 1];

    state->pc = (pc_hi << 8) | pc_lo;
    state->sp += 2;
}

void push(State8080* state, uint8_t reg_hi, uint8_t reg_lo) {
    state->memory[state->sp - 2] = reg_lo;
    state->memory[state->sp - 1] = reg_hi;
    state->sp -= 2;
}

void pop(State8080* state, uint8_t* reg_hi, uint8_t* reg_lo) {
    *reg_lo = state->memory[state->sp];
    *reg_hi = state->memory[state->sp + 1];
    state->sp += 2;
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
    state->cc.cy = (state->a < num + state->cc.cy);;
    state->cc.p = parity(result & 0xff);
    
    state->a = result & 0xff;
}

void ana(State8080* state, uint8_t num) {
    uint8_t result = state->a & num;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = 0;
    state->cc.p = parity(result);

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

State8080* init_8080_state(void)
{
	State8080* state = calloc(1, sizeof(State8080));
	state->memory = malloc(0x10000);  //16K
	return state;
}

void print_cpu_status(State8080* state) {
    printf("\n=========================== START OF CPU STATUS ===========================\n");
    printf("Registers:\n");
    printf("A:  0x%.2x    B:  0x%.2x    C:  0x%.2x\n", state->a, state->b, state->c);
    printf("D:  0x%.2x    E:  0x%.2x    H:  0x%.2x\n", state->d, state->e, state->h);
    printf("L:  0x%.2x    SP: 0x%.4x\n", state->l, state->sp);
    printf("\nFlags:\n");
    printf("Z:  0x%.2x    S:  0x%.2x    CY: 0x%.2x\n", state->cc.z, state->cc.s, state->cc.cy);
    printf("AC: 0x%.2x    P:  0x%.2x    PC: 0x%.4x\n", state->cc.ac, state->cc.p, state->pc);
    printf("\n=========================== END OF CPU STATUS ===========================\n\n");
}

void print_ram_status(State8080* state) {
    printf("\n=========================== START OF RAM STATUS ===========================\n\n");
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

    printf("\n=========================== END OF RAM STATUS ===========================\n\n");
}