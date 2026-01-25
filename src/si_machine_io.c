#include "si_machine_io.h"

/* One-shot flags: coin and start buttons are reported as "pressed" only once
 * per physical press. When the game reads IN 1, a held key would otherwise be reported
 * every poll (thousands per frame); the game can then think "start pressed
 * again" and return to title. Clear consumed when key is released. */
static bool credit_consumed = false;
static bool start1_consumed = false;
static bool start2_consumed = false;

uint8_t si_machine_in(State8080* state, uint8_t port, KeyboardMap* keyboard_state) 
{
    uint8_t input_port = 0;
    switch (port) {
        case 0:
            input_port = 0;
            // bit 0 DIP4 (self-test-request read at power up): 1 = no self-test
            input_port |= (1 << 0);
            
            // bit 1, 2, 3 Always 1
            input_port |= (1 << 1);
            input_port |= (1 << 2);
            input_port |= (1 << 3);

            // bit 4 Fire
            if (get_key_state(keyboard_state, ' '))
                input_port |= (1 << 4);

            // bit 5 Left
            if (get_key_state(keyboard_state, 'a')) {
                input_port |= (1 << 5);
            }

            // bit 6 Right
            if (get_key_state(keyboard_state, 'd')) {
                input_port |= (1 << 6);
            }

            // bit 7 ? tied to demux port 7 ?
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);

            return input_port;

        case 1: {
            input_port = 0;
            /* One-shot: only report credit/start for one read per press. */
            if (!get_key_state(keyboard_state, 'c'))
                credit_consumed = false;
            else if (!credit_consumed) {
                input_port |= (1 << 0);
                credit_consumed = true;
            }

            if (!get_key_state(keyboard_state, '2'))
                start2_consumed = false;
            else if (!start2_consumed) {
                input_port |= (1 << 1);
                start2_consumed = true;
            }

            if (!get_key_state(keyboard_state, '1'))
                start1_consumed = false;
            else if (!start1_consumed) {
                input_port |= (1 << 2);
                start1_consumed = true;
            }

            // bit 3 = Always 1
            input_port |= (1 << 3);

            // bit 4 = 1P shot (1 if pressed)
            if (get_key_state(keyboard_state, ' '))             
                input_port |= (1 << 4);

            // bit 5 = 1P left (1 if pressed)
            if (get_key_state(keyboard_state, 'a'))
                input_port |= (1 << 5);

            // bit 6 = 1P right (1 if pressed)
            if (get_key_state(keyboard_state, 'd'))
                input_port |= (1 << 6);
        
            // bit 7 = Not connected
            // Always 0 for emulation purposes
            input_port &= ~(1 << 7);
            return input_port;
        }

        case 2:
            input_port = 0;
            // bit 0 = DIP3 00 = 3 ships
            input_port &= ~(1 << 0);
            
            // bit 1 = DIP5 01 = 4 ships
            input_port |= (1 << 1);

            // bit 2 = Tilt
            input_port &= ~(1 << 2);

            // bit 3 = DIP6 0 = extra ship at 1500
            input_port |= (1 << 3);

            // bit 4 = P2 shot (1 if pressed)
            if (get_key_state(keyboard_state, 'l'))
                input_port |= (1 << 4);

            // bit 5 = P2 left (1 if pressed)
            if (get_key_state(keyboard_state, 'y'))
                input_port |= (1 << 5);

            // bit 6 = P2 right (1 if pressed)
            if (get_key_state(keyboard_state, 'i'))
                input_port |= (1 << 6);

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
    return input_port;
}

void si_machine_out(State8080* state, uint8_t port) {
    // Implement the OUT operation

    switch (port) {
        case 2:
            state->external_devices.shift_registers.shift_offset = state->a & 0x7;
            break;
        case 4:
            // Not sure if this is right.
            state->external_devices.shift_registers.shift0 = state->external_devices.shift_registers.shift1;
            state->external_devices.shift_registers.shift1 = state->a;
            break;
    
    default:
        break;
    }
}
