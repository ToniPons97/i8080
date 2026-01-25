#include "keyboard_state.h"
#include <stdio.h>
#include <string.h>

void init_keyboard_state(KeyboardMap* keyboard_state) 
{
    for (int i = 0; i < MAX_KEYS; i++) {
        keyboard_state->state[i] = false;
        keyboard_state->prev[i] = false;
    }
}

void set_key_state(KeyboardMap* keyboard_state, int key, bool pressed) 
{
    if (key >= 0 && key < MAX_KEYS)
        keyboard_state->state[key] = pressed;
}

bool get_key_state(KeyboardMap* keyboard_state, int key) 
{
    if (key >= 0 && key < MAX_KEYS)
        return keyboard_state->state[key];

    return false;
}

void keyboard_end_frame(KeyboardMap* kbd) 
{
    memcpy(kbd->prev, kbd->state, sizeof(kbd->state));
}
