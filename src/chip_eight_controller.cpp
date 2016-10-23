#include "chip_eight_controller.h"

ChipEightController::ChipEightController(ChipEight *c8) {
    chip_eight = c8;
    esc = false;
    SDL_StopTextInput();
}

bool ChipEightController::esc_pressed() {
    return esc;
}

void ChipEightController::wait_for_input() {
    bool waiting_for_event = true;
    auto waiting_register = chip_eight->get_waiting_register();

    while (waiting_for_event && !esc) {
        SDL_WaitEvent(&event);
        waiting_for_event = handle_key_event(event);
    }

    *waiting_register = last_used_key->second;
    chip_eight->set_is_waiting(false);
}

void ChipEightController::process_input() {
    if (chip_eight->get_is_waiting()) {
        wait_for_input();
    } else {
        SDL_PollEvent(&event);
        handle_key_event(event);
    }
}

bool ChipEightController::handle_key_event(SDL_Event event) {
    bool key_pressed = false;
    auto key = keymap.find(event.key.keysym.sym);
    chip_eight->get_keys().fill(0);

    switch (event.type)
    {
        case SDL_QUIT:
            esc = true;
            break;
        case SDL_KEYDOWN:
            if (key != keymap.end()) {
                if (key->first == SDLK_ESCAPE) {
                    esc = true;
                    last_used_key = key;
                    key_pressed = true;
                } else {
                    chip_eight->set_key(key->second, 1);
                    last_used_key = key;
                    key_pressed = true;
                }
            }
            break;
    }

    return key_pressed;
}
