#include <unordered_map>
#include <SDL2/SDL.h>
#include <chrono>
#include "chip_eight.h"
#include <utility>

int main(int argc, char **argv) {
    ChipEight cpu;
    cpu.load_file(argv[1]);

    // Thanks bisqwit: http://bisqwit.iki.fi/jutut/kuvat/programming_examples/chip8/chip8.cc

    // Create screen
    SDL_Window *window = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ChipEight::width*4, ChipEight::height*6, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, ChipEight::width, ChipEight::height);

    // Create a mapping of SDL keyboard symbols to Chip-8 keypad codes
    std::unordered_map<int, int> keymap {
        {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
        {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xD},
        {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
        {SDLK_5, 0x5}, {SDLK_6, 0x6}, {SDLK_7, 0x7},
        {SDLK_8, 0x8}, {SDLK_9, 0x9}, {SDLK_0, 0x0}, {SDLK_ESCAPE, -1}
    };

    uint16_t insns_per_frame = 50000;
    uint16_t max_consecutive_insns = 0;
    int frames_done = 0;
    bool interrupted = false;

    auto start = std::chrono::system_clock::now();
    while(!interrupted)
    {
        for (unsigned a = 0; a < max_consecutive_insns && !(cpu.get_waiting_key() & 0x80); ++a) {
            cpu.execute_instruction();
        }
        SDL_Event ev;
        SDL_PollEvent(&ev);
            switch(ev.type) {
                case SDL_QUIT: interrupted = true; break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    auto i = keymap.find(ev.key.keysym.sym);
                    if (i == keymap.end()) {
                        break;
                    }
                    if (i->second == -1) {
                        interrupted = true;
                        break;
                    }
                    if (ev.type == SDL_KEYDOWN && (cpu.get_waiting_key() & 0x80)) {
                        cpu.set_waiting_key(cpu.get_waiting_key() & 0x7F);
                        cpu.set_v_register(cpu.get_waiting_key(), i->second);
                    }
            }
        
        auto cur = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = cur-start;
        int frames = int(elapsed_seconds.count() * 60) - frames_done;
        if (frames > 0) {
            frames_done += frames;
            // Update the timer registers
            int st = std::min(frames, cpu.get_sound_register()+0);
            cpu.set_sound_register(cpu.get_sound_register() - st);
            int dt = std::min(frames, cpu.get_delay_register()+0);
            cpu.set_delay_register(cpu.get_delay_register() - dt);
            // Render graphics
            uint32_t pixels[ChipEight::width * ChipEight::height];
            cpu.render_to_screen(pixels);
            SDL_UpdateTexture(texture, nullptr, pixels, 4 * ChipEight::width);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
        // Adjust the instruction count to compensate for our rendering speed
        max_consecutive_insns = std::max(frames, 1) * insns_per_frame;
        // If the CPU is still waiting for a key, or if we didn't
        // have a frame yet, consume a bit of time
        if ((cpu.get_waiting_key() & 0x80) || !frames) {
            SDL_Delay(1000 / 60);
        }
    }
    SDL_Quit();

    return 0;
}
