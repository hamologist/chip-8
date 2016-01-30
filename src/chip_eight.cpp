#include "chip_eight.h"

ChipEight::ChipEight() {
    pc = 0x0200;
    sp = 0;
    sound_register = 0;
    delay_register = 0;
    memory.fill(0);
    display_memory.fill(0);
    stack.fill(0);
    v_registers.fill(0);
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

/*
 *  Executes varying instructions using the provided opcodes against the Chip-8 system
 *
 *  *NOTE*
 *    - NNN => the lowest 12 bits of the instruction
 *    - NN  => the lowest 8 bits of the instruction
 *    - X   => the lower 4 bits of the high byte of the instruction
 *    - Y   => the upper 4 bits of the low byte of the instruction
 */
void ChipEight::execute_instruction() {
    switch(i_register & 0xF000) {
        case 0x0000:
            switch(i_register) {
                case 0x00E0: // resets the display buffer
                    display_memory.fill(0);
                    break;
                case 0x00EE: // return from a subroutine
                    // TODO
                    break;
            }
            break;
        case 0x1000: // Set the program counter to 0nnn
            pc = i_register & 0x0FFF;
            break;
        case 0x2000: // Execute subroutine start at address NNN
            // TODO
            break;
        case 0x3000: // Skip the following instruction if VX == NN
            if (v_registers[i_register & 0x0F00 >> 0x0008] == (i_register & 0x00FF)) {
                pc += 4;
            }
            break;
        case 0x4000: // Skip the following instruction if VX != NN
            if (v_registers[i_register & 0x0F00 >> 0x0008] != (i_register & 0x00FF)) {
                pc += 4;
            }
            break;
        case 0x5000: // Skip the following instruction if VX == VY
            if (v_registers[i_register & 0x0F00 >> 0x0008] == v_registers[i_register & 0x00F0 >> 0x0008]) {
                pc += 4;
            }
            break;
        case 0x6000: // Sets VX = NN
            v_registers[i_register & 0x0F00 >> 0x0008] = i_register & 0x00FF;
            break;
        case 0x7000: // Sets VX = VX + NN
            v_registers[i_register & 0x0F00 >> 0x0008] += i_register & 0x00FF;
            break;
        case 0x8000: 
            switch (i_register & 0x000F) {
                case 0x0000: // Sets VX = VY
                    v_registers[i_register & 0x0F00 >> 0x0008] = v_registers[i_register & 0x00F0 >> 0x0008];
                    break;
                case 0x0001: // Sets VX = VX | VY
                    v_registers[i_register & 0x0F00 >> 0x0008] |= v_registers[i_register & 0x00F0 >> 0x0008];
                    break;
                case 0x0002: // Sets VX = VX & VY
                    v_registers[i_register & 0x0F00 >> 0x0008] &= v_registers[i_register & 0x00F0 >> 0x0008];
                    break;
                case 0x0003: // Sets VX = VX ^ VY
                    v_registers[i_register & 0x0F00 >> 0x0008] ^= v_registers[i_register & 0x00F0 >> 0x0008];
                    break;
                case 0x0004: { // Sets Vx = VX + VY and VF as carry
                    std::uint8_t x = i_register & 0x0F00 >> 0x0008;
                    std::uint8_t y = i_register & 0x00F0 >> 0x0008;
                    std::uint16_t sum = v_registers[x] + v_registers[y];
                    v_registers[0xF] = sum >> 8;
                    v_registers[x] = sum;
                    break;
                }
            }
            break;
        case 0x9000: // Skip the following instruction if VX != VY
            if (v_registers[i_register & 0x0F00 >> 0x0008] != v_registers[i_register & 0x00F0 >> 0x0008]) {
                pc += 4;
            }
            break;
        case 0xB000: // Sets the program counter to 0nnn + V0
            pc = (i_register & 0x0FFF) + v_registers[0];
            break;
        case 0xE000:
            switch (i_register & 0x00FF) {
                case (0x009E): // Skip the following instruction if the key pressed matches VX
                    // TODO
                    break;
                case (0x00A1): // Skip the following instruction if the key pressed doesn't match VX
                    // TODO
                    break;
            }
            break;
        case 0xF000:
            switch (i_register & 0x00FF) {
                case (0x0007): // Store the current value of the delay register in VX
                    v_registers[i_register & 0x0F00 >> 0x0008] = delay_register;
                    break;
                case (0x000A): // Wait for a keypress and store the result in VX
                    // TODO
                    break;
                case (0x0015): // Set the delay register to the value of VX
                    delay_register = v_registers[i_register & 0x0F00 >> 0x0008];
                    break;
                case (0x0018): // Set the sound register to the value of VX
                    sound_register = v_registers[i_register & 0x0F00 >> 0x0008];
                    break;
            }
        break;
    }
}
