#ifndef CHIP_EIGHT_CONTROLLER_H
#define CHIP_EIGHT_CONTROLLER_H
#include <unordered_map>
#include <SDL2/SDL.h>
#include "chip_eight.h"

class ChipEightController {
    public:
        ChipEightController(ChipEight *c8);
        bool esc_pressed();
        void process_input();

    private:
        ChipEight *chip_eight;
        SDL_Event event;
        bool esc;
        std::unordered_map<int, std::uint16_t>::const_iterator last_used_key;
        const std::unordered_map<int, std::uint16_t> keymap {
            {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
            {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
            {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
            {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
            {SDLK_ESCAPE, -1}
        };
        bool handle_key_event(SDL_Event event);
        void wait_for_input();
};
#endif //CHIP_EIGHT_CONTROLLER_H
