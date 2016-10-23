#ifndef CHIP_EIGHT_DISPLAY_H
#define CHIP_EIGHT_DISPLAY_H
#include <SDL2/SDL.h>
#include "chip_eight.h"

class ChipEightDisplay {
    public:
        const std::string window_name = "Chip-8";

        ChipEightDisplay(ChipEight *c8);
        ~ChipEightDisplay();
        void render_frame();

    private:
        ChipEight *chip_eight;
        uint16_t height;
        uint16_t width;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Rect pixel;

        void clear_no_draw();
};
#endif //CHIP_EIGHT_DISPLAY_H
