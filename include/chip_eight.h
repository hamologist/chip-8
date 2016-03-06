#ifndef CHIP_EIGHT_H
#define CHIP_EIGHT_H
#include <iostream>
#include <cstdint>
#include <array>
#include <random>
#include <chrono>

class ChipEight {
    public:
        const static std::uint16_t width = 64;
        const static std::uint16_t height = 32;
        const static std::uint16_t display_size = width * height / 8;

        ChipEight ();
        const std::array<std::uint8_t, 4096> &get_memory();
        const std::array<std::uint8_t, display_size> &get_display_memory();
        const std::array<std::uint8_t, 16> &get_v_registers();
        std::uint16_t get_pc();
        void set_i_register(std::uint16_t value);
        void execute_instruction();

    private:
        std::array<std::uint8_t, 4096> memory; 
        std::array<std::uint8_t, display_size> display_memory;
        std::array<std::uint16_t, 16> stack;
        std::array<std::uint8_t, 16> v_registers;

        std::uint16_t pc;
        std::uint8_t sp;
        std::uint16_t i_register;
        std::uint8_t sound_register;
        std::uint8_t delay_register;
};
#endif //CHIP_EIGHT_H
