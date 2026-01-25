#include "display.h"

void initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

SDL_Window* create_window() {
    SDL_Window *window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return renderer;
}

void render_screen(uint8_t *memory, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int addr = VRAM_START; addr <= VRAM_END; addr++) {
        uint8_t byte = memory[addr];
        
        for (int bit = 0; bit < 8; bit++) {
            if (byte & (1 << bit)) {
                int y = ((addr - VRAM_START) * 8 + bit) / 256;
                int x = ((addr - VRAM_START) * 8 + bit) % 256;
                
                // Adjust for -90 degree rotation
                int rotated_x = y * SCALE;  // Swap x and y
                int rotated_y = (255 - x) * SCALE;  // Flip y-axis

                SDL_Rect rect = { rotated_x, rotated_y, SCALE, SCALE };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void handle_quit_event(SDL_Event* event, bool* quit) {
    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            *quit = true;
        }
    }
}

void handle_sdl_events(KeyboardMap* keyboard_state, SDL_Event* event, bool* quit) {
    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            *quit = true;
            break;
        }

        if (keyboard_state != NULL) {
            handle_keyboard_events(keyboard_state, event);
        }
    }
}

// SPACE (fire) | a (left) | d (right) |  c (coin insertion) | y (p2 left) | i (p2 right) | l (p2 fire)
void handle_keyboard_events(KeyboardMap* keyboard_state, SDL_Event* event) {
    SDL_KeyCode keycode = event->key.keysym.sym;
    if (event->type == SDL_KEYDOWN) {
        if (keycode == SDLK_1 ||
            keycode == SDLK_SPACE || 
            keycode == SDLK_a || 
            keycode == SDLK_d || 
            keycode == SDLK_c ||
            keycode == SDLK_l ||
            keycode == SDLK_y ||
            keycode == SDLK_i) {
            set_key_state(keyboard_state, keycode, true);
        }
    }

    if (event->type == SDL_KEYUP) {
        if (keycode == SDLK_1 ||
            keycode == SDLK_SPACE || 
            keycode == SDLK_a || 
            keycode == SDLK_d || 
            keycode == SDLK_c ||
            keycode == SDLK_l ||
            keycode == SDLK_y ||
            keycode == SDLK_i) {
            set_key_state(keyboard_state, keycode, false);
        }
    }
}

void sdl_cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    /* Destroy renderer before window; destroying the window can invalidate
     * the renderer and cause double-free if we destroy in wrong order. */
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
