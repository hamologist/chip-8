#include "chip_eight.h"

ChipEight::ChipEight() {
    pc = 0x0200;
    sp = 0;
    sound_register = 0;
    delay_register = 0;
    memory.fill(0);
    display_memory.fill(0);
}

const std::array<std::uint8_t, 4096> &ChipEight::get_memory() {
    return memory;
}

const std::array<std::uint8_t, ChipEight::display_size> &ChipEight::get_display_memory() {
    return display_memory;
}

std::uint16_t ChipEight::get_pc() {
    return pc;
}

void ChipEight::set_i_register(std::uint16_t value) {
    i_register = value;
}

void ChipEight::execute_instruction() {
    switch(i_register & 0xF000) {
        case 0x0000:
            switch(i_register) {
                case 0x00E0: 
                    display_memory.fill(0);
                    break;
                case 0x00EE:
                    break;
            }
        case 0x1000:
            pc = i_register & 0x0FFF;
            break;
    }
}
