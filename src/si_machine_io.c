#include "si_machine_io.h"

uint8_t si_machine_in(State8080* state, uint8_t port, KeyboardMap* keyboard_state) {
    uint8_t input_port = 0;
    switch (port) {
        case 0:
            // bit 0 DIP4 (Seems to be self-test-request read at power up)
            // Always 1 for emulation purposes
            input_port |= (1 << 0);
            
            // bit 1, 2, 3 Always 1
            input_port |= (1 << 1);
            input_port |= (1 << 2);
            input_port |= (1 << 3);

            // bit 4 Fire
            if (get_key_state(keyboard_state, ' ')) {
                input_port |= (1 << 4);
            } else {
                input_port &= ~(1 << 4);
            }

            // bit 5 Left
            if (get_key_state(keyboard_state, 'a')) {
                input_port |= (1 << 5);
            } else {
                input_port &= ~(1 << 5);
            }

            // bit 6 Right
            if (get_key_state(keyboard_state, 'd')) {
                input_port |= (1 << 6);
            } else {
                input_port &= ~(1 << 6);
            }

            // bit 7 ? tied to demux port 7 ?
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);

            return input_port;

        case 1:
            // bit 0 = CREDIT (1 if deposit)
            if (get_key_state(keyboard_state, 'c')) {
                input_port |= (1 << 0);
            } else {
                input_port &= ~(1 << 0);
            }

            // bit 1 = 2P start (1 if pressed)
            if (get_key_state(keyboard_state, '1')) {
                input_port |= (1 << 1);
            } else {
                input_port &= ~(1 << 1);
            }

            // bit 2 = 1P start (1 if pressed)
            if (get_key_state(keyboard_state, '2')) {
                input_port |= (1 << 2);
            } else {
                input_port &= ~(1 << 2);
            }
            
            // bit 3 = Always 1
            input_port |= (1 << 3);

            // bit 4 = 1P shot (1 if pressed)
            if (get_key_state(keyboard_state, ' ')) {
                input_port |= (1 << 4);
            } else {
                input_port &= ~(1 << 4);
            }

            // bit 5 = 1P left (1 if pressed)
            if (get_key_state(keyboard_state, 'a')) {
                input_port |= (1 << 5);
            } else {
                input_port &= ~(1 << 5);
            }

            // bit 6 = 1P right (1 if pressed)
            if (get_key_state(keyboard_state, 'd')) {
                input_port |= (1 << 6);
            } else {
                input_port &= ~(1 << 6);
            }
        
            // bit 7 = Not connected
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);

            return input_port;

        case 2:
            // bit 0 = DIP3 00 = 3 ships
            input_port &= ~(1 << 0);
            
            // bit 1 = DIP5 01 = 4 ships
            input_port |= (1 << 1);

            // bit 2 = Tilt
            input_port &= ~(1 << 2);

            // bit 3 = DIP6 0 = extra ship at 1500
            input_port |= (1 << 3);

            // bit 4 = P2 shot (1 if pressed)
            if (get_key_state(keyboard_state, 'l')) {
                input_port |= (1 << 4);
            } else {
                input_port &= ~(1 << 4);
            }

            // bit 5 = P2 left (1 if pressed)
            if (get_key_state(keyboard_state, 'y')) {
                input_port |= (1 << 5);
            } else {
                input_port &= ~(1 << 5);
            }

            // bit 6 = P2 right (1 if pressed)
            if (get_key_state(keyboard_state, 'i')) {
                input_port |= (1 << 6);
            } else {
                input_port &= ~(1 << 6);
            }

            // bit 7 = DIP7 Coin info displayed in demo screen 0=ON
            input_port |= (1 << 7);

            return input_port;
            
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

    switch (port) {
        case 4:
            // Not sure if this is right.
            state->external_devices.shift_registers.shift0 = state->external_devices.shift_registers.shift1;
            state->external_devices.shift_registers.shift1 = state->a;
            break;
    
    default:
        break;
    }


}