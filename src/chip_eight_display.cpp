#include "chip_eight_display.h"

ChipEightDisplay::ChipEightDisplay(ChipEight *c8) {
    chip_eight = c8;
    width = c8->width;
    height = c8->height;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        window_name.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        c8->width*10,
        c8->height*10,
        SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    pixel.w = 10;
    pixel.h = 10;
}

void ChipEightDisplay::clear_no_draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void ChipEightDisplay::render_frame() {
    clear_no_draw();

    auto display_memory = chip_eight->get_display_memory();
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (display_memory[y * 64 + x] == 1)
            {
                pixel.x = x * 10;
                pixel.y = y * 10;

                SDL_RenderFillRect(renderer, &pixel);
            }

        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

ChipEightDisplay::~ChipEightDisplay() {
    SDL_Quit();
}
