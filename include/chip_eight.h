#include <iostream>
#include <cstdint>
#include <array>

class ChipEight {
    std::array<std::uint8_t, 4096> memory; 
    std::array<std::uint16_t, 16> stack;
    std::array<std::uint8_t, 16> v_registers;

    std::uint16_t pc;
    std::uint8_t sp;
    std::uint16_t i_register;
    std::uint8_t sound_register;
    std::uint8_t delay_register;
    public:
        ChipEight ();
};
