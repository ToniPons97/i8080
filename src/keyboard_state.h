#ifndef KEYBOARD_STATE_H
#define KEYBOARD_STATE_H

#include <stdbool.h>

#define MAX_KEYS 256

typedef struct {
    bool state[MAX_KEYS];
} KeyboardMap;

void init_keyboard_state(KeyboardMap* keyboard_state);
void set_key_state(KeyboardMap* keyboard_state, int key, bool pressed);
bool get_key_state(KeyboardMap* keyboard_state, int key);

#endif