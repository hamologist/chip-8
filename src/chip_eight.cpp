#include "chip_eight.h"

ChipEight::ChipEight() {
}

std::array<std::uint8_t, 4096> ChipEight::get_memory() {
    return memory;
}
