#ifndef DISPLAY
#define DISPLAY

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "common_values.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 324
#define SCALE 3

void initialize_sdl();
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void render_screen(uint8_t *memory, SDL_Renderer *renderer);
void sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer);
void handle_quit_event(SDL_Event* event, bool* quit);

#endif