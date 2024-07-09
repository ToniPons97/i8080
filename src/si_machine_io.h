#ifndef SI_MACHINE_IO
#define SI_MACHINE_IO

#include "state8080.h"
#include "keyboard_state.h"

uint8_t si_machine_in(State8080* state, uint8_t port, KeyboardMap* keyboard_state) {
    uint8_t input_port = 0;
    switch (port) {
        case 0:
            
            // bit 0 DIP4 (Seems to be self-test-request read at power up)
            // Always 1 for emulation purposes
            input_port |= (1 << 0);
            
            // bit 1 Always 1
            input_port |= (1 << 1);
            
            //  bit 2 Always 1
            input_port |= (1 << 2);

            // bit 3 Always 1
            input_port |= (1 << 3);


            /*
                bit 4 Fire
                bit 5 Left
                bit 6 Right
            */




            // bit 7 ? tied to demux port 7 ?
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);

            return input_port;

        case 1:
            /*
                bit 0 = CREDIT (1 if deposit)
                bit 1 = 2P start (1 if pressed)
                bit 2 = 1P start (1 if pressed)
                bit 4 = 1P shot (1 if pressed)
                bit 5 = 1P left (1 if pressed)
                bit 6 = 1P right (1 if pressed)
            */

            if (get_key_state(keyboard_state, 'c')) {
                input_port |= (1 << 0);
            } else {
                input_port &= ~(1 << 0);
            }
            
            // bit 3 = Always 1
            input_port |= (1 << 3);
        
            // bit 7 = Not connected
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);

        case 3:
            uint16_t a = 0;
            uint16_t v = (state->external_devices.shift_registers.shift1 << 8) | state->external_devices.shift_registers.shift0;    
            
            a = ((v >> (8 - state->external_devices.shift_registers.shift_offset)) & 0xff);
            return a & 0xff;

        default:
            break;
    }
    // Implement the IN operation
    return 0;
}

void si_machine_out(State8080* state, uint8_t port) {
    // Implement the OUT operation
}

#endif