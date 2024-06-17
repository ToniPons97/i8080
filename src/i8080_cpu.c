#include <stdio.h>
#include <stdlib.h>
#include "i8080_cpu.h"

// https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf

void unimplemented_instruction(State8080* state) {    
    state->pc -= 1;

    printf ("Error: Unimplemented instruction\n");    
    exit(1);    
}

void emulate_i8080_op(State8080* state) {    
    unsigned char *opcode = &state->memory[state->pc];    

    switch(*opcode) {    
        case 0x00: break;           // NOP    
        case 0x01:                  // LXI B,D16
            state->c = opcode[1];
            state->b = opcode[2];
            state->pc += 2;
            break;    
        case 0x02:                  // STAX B
            state->b = opcode[1];
            state->c = opcode[2];
            break;    
        case 0x03:                  // 	INX B
            uint16_t bc = (state->b << 8) | state->c;
            bc += 1;
            state->b = (bc >> 8) & 0xff;
            state->c = bc & 0xff;
            break;    
        case 0x04: unimplemented_instruction(state); break;    
        case 0x05: unimplemented_instruction(state); break;    
        case 0x06:                  // MVI B, D8
            state->b = opcode[1];
            state->pc += 1;
            break;    
        case 0x07: unimplemented_instruction(state); break;    
        case 0x08: unimplemented_instruction(state); break;    
        case 0x09: unimplemented_instruction(state); break;    
        case 0x0a:                  // LDAX B 
            uint16_t address = (state->b << 8) | state->c;
            state->a = state->memory[address];
            break;    
        case 0x0b:                  // DCX B
            uint16_t bc = (state->b << 8) | state->c;
            bc -= 1;
            state->b = (state->b >> 8) & 0xff;
            state->c = bc & 0xff;
            break;    
        case 0x0c: unimplemented_instruction(state); break;    
        case 0x0d: unimplemented_instruction(state); break;    
        case 0x0e:                  // MVI C,D8 
            state->c = opcode[1];
            state->pc += 1;
            break;    
        case 0x0f: unimplemented_instruction(state); break;    
        case 0x10: unimplemented_instruction(state); break;    
        case 0x11:                  // LXI D,D16
            state->d = opcode[2];
            state->e = opcode[1];
            state->pc += 2;
            break;    
        case 0x12:                  // STAX D
            uint16_t address = (state->d << 8) | state->e;
            state->memory[address] = state->a;
            break;    
        case 0x13:                  // INX D
            uint16_t de = (state->d << 8) | state->e;
            de += 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            break;    
        case 0x14: unimplemented_instruction(state); break;    
        case 0x15: unimplemented_instruction(state); break;    
        case 0x16:                  // MVI D, D8
            state->d = opcode[1];
            state->pc += 1;
            break;    
        case 0x17: unimplemented_instruction(state); break;    
        case 0x18: unimplemented_instruction(state); break;    
        case 0x19: unimplemented_instruction(state); break;    
        case 0x1a:                  // LDAX D 
            uint16_t address = (state->d << 8) | state->e;
            state->a = state->memory[address];
            break;
        case 0x1b:                  // 	DCX D
            uint16_t de = (state->d << 8) | state->e;
            de -= 1;
            state->d = (de >> 8) & 0xff;
            state->e = de & 0xff;
            break;    
        case 0x1c: unimplemented_instruction(state); break;    
        case 0x1d: unimplemented_instruction(state); break;    
        case 0x1e:                  // MVI E,D8 
            state->e = opcode[1];
            state->pc += 1;
            break;    
        case 0x1f: unimplemented_instruction(state); break;    
        case 0x20: unimplemented_instruction(state); break;    
        case 0x21:                  // 	LXI H,D16
            state->h = opcode[2];
            state->l = opcode[1];
            state->pc += 2;
            break;    
        case 0x22:                  // SHLD adr
            uint16_t address = (opcode[1] << 8) | opcode[1];
            state->memory[address] = state->l;
            state->memory[address + 1] = state->h;
            state->pc += 2;
            break;    
        case 0x23:                  // INX H
            uint16_t hl = (state->h << 8) | state->l;
            hl += 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            break;    
        case 0x24: unimplemented_instruction(state); break;    
        case 0x25: unimplemented_instruction(state); break;    
        case 0x26:                  // MVI H,D8 
            state->h = opcode[1];
            state->pc += 1;
            break;    
        case 0x27: unimplemented_instruction(state); break;    
        case 0x28: unimplemented_instruction(state); break;    
        case 0x29: unimplemented_instruction(state); break;    
        case 0x2a:                  // LHLD adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->l = state->memory[address];
            state->h = state->memory[address + 1];
            state->pc += 2;
            break;    
        case 0x2b:                  // 	DCX H
            uint16_t hl = (state->h << 8) | state->l;
            hl -= 1;
            state->h = (hl >> 8) & 0xff;
            state->l = hl & 0xff;
            break;    
        case 0x2c: unimplemented_instruction(state); break;    
        case 0x2d: unimplemented_instruction(state); break;    
        case 0x2e:                  // MVI L, D8 
            state->l = opcode[1];
            state->pc += 1;
            break;    
        case 0x2f:                  // CMA
            state->a = ~state->a;
            break;    
        case 0x30: unimplemented_instruction(state); break;    
        case 0x31:                  // 	LXI SP, D16
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->sp = address;
            state->pc += 2;
            break;    
        case 0x32:                  // STA adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->memory[address] = state->a;
            state->pc += 2;
            break;    
        case 0x33:                  // INX SP
            state->sp += 1;
            break;    
        case 0x34: unimplemented_instruction(state); break;    
        case 0x35: unimplemented_instruction(state); break;    
        case 0x36:                  // MVI M,D8
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = opcode[1];
            state->pc += 1;
            break;
        case 0x37:                  // STC
            state->cc.cy = 1;
            break;    
        case 0x38: break;    
        case 0x39: unimplemented_instruction(state); break;    
        case 0x3a:                  // LDA adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->a = state->memory[address];
            state->pc += 2;
            break;    
        case 0x3b:                  // 	DCX SP
            state->sp -= 1;
            break;    
        case 0x3c: unimplemented_instruction(state); break;    
        case 0x3d: unimplemented_instruction(state); break;    
        case 0x3e:                  // 	MVI A,D8
            state->a = opcode[1];
            state->pc += 1;
            break;    
        case 0x3f:                  // CMC
            state->cc.cy = ~state->cc.cy;
            break;    
        case 0x40:                  // MOV B,B
            state->b = state->b;
            break;    
        case 0x41:                  // MOV B,C
            state->b = state->c;
            break;    
        case 0x42:                  // 	MOV B,D
            state->b = state->d;
            break;    
        case 0x43:                  // 	MOV B,E
            state->b = state->e;
            break;    
        case 0x44:                  // MOV B,H
            state->b = state->h;
            break;    
        case 0x45:                  // 	MOV B,L
            state->b = state->l;
            break;    
        case 0x46:                  // 	MOV B,M
            uint16_t address = (state->h << 8) | state->l;
            state->b = state->memory[address];
            break;    
        case 0x47:                  // MOV B,A
            state->b = state->a;
            break;    
        case 0x48:                  // 	MOV C,B
            state->c = state->b;
            break;    
        case 0x49:                  // 	MOV C,C
            state->c = state->c;
            break;    
        case 0x4a:                  // 	MOV C,D
            state->c = state->d;
            break;    
        case 0x4b:                  // MOV C,E
            state->c = state->e;
            break;    
        case 0x4c:                  // MOV C,H
            state->c = state->h;
            break;    
        case 0x4d:                  // 	MOV C,L
            state->c = state->l;
            break;    
        case 0x4e:                  // MOV C,M
            uint16_t address = (state->h << 8) | state->l;
            state->c = state->memory[address];
            break;    
        case 0x4f:                  // MOV C,A
            state->c = state->a;
            break;    
        case 0x50:                  // 	MOV D,B
            state->d = state->b;
            break;    
        case 0x51:                  // MOV D,C
            state->d = state->c;
            break;    
        case 0x52:                  // 	MOV D,D 
            state->d = state->d;
            break;    
        case 0x53:                  // 	MOV D,E
            state->d = state->e;
            break;    
        case 0x54:                  // MOV D,H
            state->d = state->h;
            break;    
        case 0x55:                  // MOV D,L
            state->d = state->l;
            break;    
        case 0x56:                  // 	MOV D,M
            uint16_t address = (state->h << 8) | state->l;
            state->d = state->memory[address];
            break;    
        case 0x57:                  // 	MOV D,A
            state->d = state->a;
            break;    
        case 0x58:                  // 	MOV E,B
            state->e = state->b;
            break;    
        case 0x59:                  // 	MOV E,C
            state->e = state->c;
            break;    
        case 0x5a:                  // MOV E,D
            state->e = state->d;
            break;    
        case 0x5b:                  // MOV E,E
            state->e = state->e;
            break;    
        case 0x5c:                  // 	MOV E,H
            state->e = state->h;
            break;    
        case 0x5d:                  // MOV E,L
            state->e = state->l;
            break;    
        case 0x5e:                  // 	MOV E,M
            uint16_t address = (state->h << 8) | state->l;
            state->e = state->memory[address];
            break;    
        case 0x5f:                  // 	MOV E,A
            state->e = state->a;
            break;    
        case 0x60:                  // 	MOV H,B
            state->h = state->b;
            break;    
        case 0x61:                  // 	MOV H,C
            state->h = state->c;
            break;    
        case 0x62:                  // 	MOV H,D
            state->h = state->b;
            break;    
        case 0x63:                  // 	MOV H,E
            state->h = state->e;
            break;    
        case 0x64:                  // 	MOV H,H
            state->h = state->h;
            break;    
        case 0x65:                  // 	MOV H,L
            state->h = state->l;
            break;    
        case 0x66:                  // 	MOV H,M
            uint16_t address = (state->h << 8) | state->l;
            state->h = state->memory[address];
            break;    
        case 0x67:                  // 	MOV H,A
            state->h = state->a;
            break;    
        case 0x68:                  // 	MOV L,B
            state->l = state->b;
            break;    
        case 0x69:                  // 	MOV L,C
            state->l = state->c;
            break;    
        case 0x6a:                  // MOV L,D
            state->l = state->d;
            break;    
        case 0x6b:                  // 	MOV L,E
            state->l = state->e;
            break;    
        case 0x6c:                  // 	MOV L,H
            state->l = state->h;
            break;    
        case 0x6d:                  // 	MOV L,L
            state->l = state->l;
            break;    
        case 0x6e:                  // MOV L,M
            uint16_t address = (state->h << 8) | state->l;
            state->l = state->memory[address];
            break;    
        case 0x6f:                  // 	MOV L,A
            state->l = state->a;
            break;    
        case 0x70:                  // 	MOV M,B
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->b;
            break;    
        case 0x71:                  // MOV M,C
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->c;
            break;    
        case 0x72:                  // 	MOV M,D
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->d;
            break;    
        case 0x73:                  // 	MOV M,E
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->e;
            break;    
        case 0x74:                  // MOV M,H
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->h;
            break;    
        case 0x75:                  // 	MOV M,L
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->l;
            break;    
        case 0x76:                  // HLT
            exit(0); 
            break;    
        case 0x77:                  // 	MOV M,L
            uint16_t address = (state->h << 8) | state->l;
            state->memory[address] = state->l;
            break;    
        case 0x78:                  // MOV A,B
            state->a = state->b;
            break;    
        case 0x79:                  // 	MOV A,C
            state->a = state->c;
            break;    
        case 0x7a:                  // 	MOV A,D
            state->a = state->d;
            break;    
        case 0x7b:                  // MOV A,E
            state->a = state->e;
            break;    
        case 0x7c:                  // 	MOV A,H
            state->a = state->h;
            break;    
        case 0x7d:                  // 	MOV A,L
            state->a = state->l;
            break;    
        case 0x7e:                  // MOV A,M
            uint16_t address = (state->h << 8) | state->l;
            state->a = state->memory[address];
            break;    
        case 0x7f:                  // MOV A,A
            state->a = state->a;
            break;    
        case 0x80:                  // ADD B 
            add_to_accumulator(state, (uint16_t) state->b);
            break;    
        case 0x81:                  // ADD C
            add_to_accumulator(state, (uint16_t) state->c);
            break;    
        case 0x82:                  // 	ADD D
            add_to_accumulator(state, (uint16_t) state->d);
            break;    
        case 0x83:                  // 	ADD E
            add_to_accumulator(state, (uint16_t) state->e);
            break;    
        case 0x84:                  // ADD H
            add_to_accumulator(state, (uint16_t) state->h);
            break;    
        case 0x85:                  // 	ADD L
            add_to_accumulator(state, (uint16_t) state->l);
            break;    
        case 0x86:                  // 	ADD M
            uint16_t address = (state->h << 8) | state->l;
            add_to_accumulator(state, (uint16_t) state->memory[address]);
            break;    
        case 0x87:                  // 	ADD A
            add_to_accumulator(state, (uint16_t) state->a);
            break;    
        case 0x88:                  // ADC B
            adc(state, (uint16_t) state->b);
            break;    
        case 0x89: unimplemented_instruction(state); break;    
        case 0x8a: unimplemented_instruction(state); break;    
        case 0x8b: unimplemented_instruction(state); break;    
        case 0x8c: unimplemented_instruction(state); break;    
        case 0x8d: unimplemented_instruction(state); break;    
        case 0x8e: unimplemented_instruction(state); break;    
        case 0x8f: unimplemented_instruction(state); break;    
        case 0x90: unimplemented_instruction(state); break;    
        case 0x91: unimplemented_instruction(state); break;    
        case 0x92: unimplemented_instruction(state); break;    
        case 0x93: unimplemented_instruction(state); break;    
        case 0x94: unimplemented_instruction(state); break;    
        case 0x95: unimplemented_instruction(state); break;    
        case 0x96: unimplemented_instruction(state); break;    
        case 0x97: unimplemented_instruction(state); break;    
        case 0x98: unimplemented_instruction(state); break;    
        case 0x99: unimplemented_instruction(state); break;    
        case 0x9a: unimplemented_instruction(state); break;    
        case 0x9b: unimplemented_instruction(state); break;    
        case 0x9c: unimplemented_instruction(state); break;    
        case 0x9d: unimplemented_instruction(state); break;    
        case 0x9e: unimplemented_instruction(state); break;    
        case 0x9f: unimplemented_instruction(state); break;    
        case 0xa0: unimplemented_instruction(state); break;    
        case 0xa1: unimplemented_instruction(state); break;    
        case 0xa2: unimplemented_instruction(state); break;    
        case 0xa3: unimplemented_instruction(state); break;    
        case 0xa4: unimplemented_instruction(state); break;    
        case 0xa5: unimplemented_instruction(state); break;    
        case 0xa6: unimplemented_instruction(state); break;    
        case 0xa7: unimplemented_instruction(state); break;    
        case 0xa8: unimplemented_instruction(state); break;    
        case 0xa9: unimplemented_instruction(state); break;    
        case 0xaa: unimplemented_instruction(state); break;    
        case 0xab: unimplemented_instruction(state); break;    
        case 0xac: unimplemented_instruction(state); break;    
        case 0xad: unimplemented_instruction(state); break;    
        case 0xae: unimplemented_instruction(state); break;    
        case 0xaf: unimplemented_instruction(state); break;    
        case 0xb0: unimplemented_instruction(state); break;    
        case 0xb1: unimplemented_instruction(state); break;    
        case 0xb2: unimplemented_instruction(state); break;    
        case 0xb3: unimplemented_instruction(state); break;    
        case 0xb4: unimplemented_instruction(state); break;    
        case 0xb5: unimplemented_instruction(state); break;    
        case 0xb6: unimplemented_instruction(state); break;    
        case 0xb7: unimplemented_instruction(state); break;    
        case 0xb8: unimplemented_instruction(state); break;    
        case 0xb9: unimplemented_instruction(state); break;    
        case 0xba: unimplemented_instruction(state); break;    
        case 0xbb: unimplemented_instruction(state); break;    
        case 0xbc: unimplemented_instruction(state); break;    
        case 0xbd: unimplemented_instruction(state); break;    
        case 0xbe: unimplemented_instruction(state); break;    
        case 0xbf: unimplemented_instruction(state); break;    
        case 0xc0:                  // RNZ
            if (!state->cc.z) {
                ret(state);
            }
            break;    
        case 0xc1:                  // 	POP B
            pop(state, &state->b, &state->c);
            break;    
        case 0xc2:                  // JNZ adr
            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    
        case 0xc3:                  // JMP adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            state->pc = address;
            break;    
        case 0xc4:                  // CNZ adr
            if (!state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xc5:                  // 	PUSH B
            push(state, state->b, state->c);
            break;    
        case 0xc6: unimplemented_instruction(state); break;    
        case 0xc7:                  // 	RST 0
            call(state, 0x00);
            break;    
        case 0xc8:                  // RZ
            if (state->cc.z) {
                ret(state);
            }
            break;    
        case 0xc9:                  // RET
            ret(state);
            break;    
        case 0xca:                  // JZ adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    
        case 0xcb: unimplemented_instruction(state); break;    
        case 0xcc:                  // CZ adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xcd:                  // CALL adr
            uint16_t address = (opcode[2] << 8) | opcode[1];
            call(state, address);
            break;
        case 0xce: unimplemented_instruction(state); break;    
        case 0xcf:                  // 	RST 1
            call(state, 0x8);
            break;    
        case 0xd0:                  // RNC
            if (!state->cc.cy) {
                ret(state);
            }
            break;    
        case 0xd1:                  // POP D
            pop(state, &state->d, &state->e);
            break;    
        case 0xd2:                  // 	JNC adr
            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }      
            break;    
        case 0xd3: unimplemented_instruction(state); break;    
        case 0xd4:                  // CNC adr
            if (!state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xd5:                  // PUSH D
            push(state, state->d, state->e);
            break;    
        case 0xd6: unimplemented_instruction(state); break;    
        case 0xd7:                  // RST 2
            call(state, 0x10);
            break;    
        case 0xd8:                  // RC
            if (state->cc.cy) {
                ret(state);
            }
            break;    
        case 0xd9: unimplemented_instruction(state); break;    
        case 0xda:                  // JC adr
            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    

        case 0xdb: unimplemented_instruction(state); break;    
        case 0xdc:                  // 	CC adr
            if (state->cc.cy) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xdd: unimplemented_instruction(state); break;    
        case 0xde: unimplemented_instruction(state); break;    
        case 0xdf:                  // RST 3
            call(state, 0x18);
            break;    
        case 0xe0:                  // RPO
            if (!state->cc.p) {
                ret(state);
            }
            break;    
        case 0xe1:                  // POP H
            pop(state, &state->h, &state->l);
            break;    
        case 0xe2:                  // 	JPO adr
            if(!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    
        case 0xe3:                  // XTHL
            // L <-> (SP); H <-> (SP+1)

            break;    
        case 0xe4:                  // CPO adr
            if (!state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xe5:                  // PUSH H
            push(state, state->h, state->l);
            break;    
        case 0xe6: unimplemented_instruction(state); break;    
        case 0xe7:                  // 	RST 4
            call(state, 0x20);
            break;    
        case 0xe8:                  // RPE
            if (state->cc.p) {
                ret(state);
            }
            break;    
        case 0xe9:                  // PCHL
            uint16_t hl = (state->h << 8) | state->l;
            state->pc = hl;
            break;    
        case 0xea:                  // 	JPE adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    
        case 0xeb:                  // XCHG
            uint8_t temp = state->h;
            state->h = state->d;
            state->d = temp;

            temp = state->l;
            state->l = state->e;
            state->e = temp;
            break;    
        case 0xec:                  // CPE adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xed: break;    
        case 0xee: unimplemented_instruction(state); break;    
        case 0xef:
            call(state, 0x28);
            break;    
        case 0xf0:                  // RP
            if (!state->cc.z) {
                ret(state);
            }
            break;    
        case 0xf1: unimplemented_instruction(state); break;    
        case 0xf2:                  // 	JP adr
            if (state->cc.z) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                state->pc = address;
            } else {
                state->pc += 2;
            }
            break;    
        case 0xf3: break;    
        case 0xf4:                  // CP adr
            if (state->cc.p) {
                uint16_t address = (opcode[2] << 8) | opcode[1];
                call(state, address);
            } else {
                state->pc += 2;
            }
            break;    
        case 0xf5: unimplemented_instruction(state); break;    
        case 0xf6:                  // ORI D8
            state->a = state->a | opcode[1];
            break;    
        case 0xf7:                  // RST 6
            call(state, 0x30);
            break;    
        case 0xf8: unimplemented_instruction(state); break;    
        case 0xf9:                  // SPHL
            uint16_t address = (state->h << 8) | state->l;
            state->sp = address;
            break;    
        case 0xfa: unimplemented_instruction(state); break;    
        case 0xfb: unimplemented_instruction(state); break;    
        case 0xfc: unimplemented_instruction(state); break;    
        case 0xfd: unimplemented_instruction(state); break;    
        case 0xfe: unimplemented_instruction(state); break;    
        case 0xff:                  // 	RST 7
            call(state, 0x38);
            break;    
    }    
    state->pc += 1;
    return;
}

void call(State8080* state, uint16_t address) {
    uint16_t pc_hi = (state->pc >> 8) & 0xff;
    uint16_t pc_lo = state->pc & 0xff;
    
    state->memory[state->sp - 1] = pc_hi;
    state->memory[state->sp - 2] = pc_lo;
    state->sp += 2;
    state->pc = address;
}

void ret(State8080* state) {
    uint16_t pc_lo = state->memory[state->sp];
    uint16_t pc_hi = state->memory[state->sp + 1];

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

void add_to_accumulator(State8080* state, uint16_t num) {
    uint16_t result = (uint16_t) state->a + num;
    
    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = result > 0xff;
    state->cc.p = parity(result & 0xff);
    
    state->a = result &0xff;
}

void adc(State8080* state, uint16_t num) {
    uint16_t result = (uint16_t) state->a + num + (uint16_t) state->cc.cy;

    state->cc.z = (result & 0xff) == 0;
    state->cc.s = (result & 0x80) != 0;
    state->cc.cy = result > 0xff;
    state->cc.p = parity(result & 0xff);
    
    state->a = result & 0xff;
}