#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef RENDERER
#define RENDERER

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256
#define SCALE 3
#define VRAM_START 0x2400
#define VRAM_END 0x3fff

void initialize_sdl();
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void render_screen(uint8_t *memory, SDL_Renderer *renderer);
void sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer);

#endif