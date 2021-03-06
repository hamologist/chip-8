#include "chip_eight.h"

ChipEight::ChipEight() {
    pc = 0x200;
    opcode = 0x200;
    waiting_register = nullptr;
    is_waiting = false;
    sp = 0;
    sound_register = 0;
    delay_register = 0;
    i_register = 0;
    keys.fill(0);
    memory.fill(0);
    display_memory.fill(0);
    stack.fill(0);
    v_registers.fill(0);

    fonts = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++) {
        memory[i] = fonts[i];
    }
}

const std::array<std::uint8_t, 4096> &ChipEight::get_memory() {
    return memory;
}

const std::array<std::uint8_t, ChipEight::display_size> &ChipEight::get_display_memory() {
    return display_memory;
}

const std::array<std::uint8_t, 16> &ChipEight::get_v_registers() {
    return v_registers;
}

std::array<std::uint8_t, 16> &ChipEight::get_keys() {
    return keys;
}

std::uint16_t ChipEight::get_pc() {
    return pc;
}

std::uint8_t *ChipEight::get_waiting_register() {
    return waiting_register;
}

std::uint8_t ChipEight::get_sound_register() {
    return sound_register;
}

std::uint8_t ChipEight::get_delay_register() {
    return delay_register;
}

bool ChipEight::get_is_waiting() {
    return is_waiting;
}

std::uint8_t get_random_byte() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine random_number_generator(seed);
    std::uniform_int_distribution<std::uint8_t> distribution(0, 255);

    return distribution(random_number_generator);
}

void ChipEight::set_i_register(std::uint16_t value) {
    i_register = value;
}

void ChipEight::set_v_register(std::uint16_t pos, std::uint16_t value) {
    v_registers[pos] = value;
}

void ChipEight::set_sound_register(std::uint8_t value) {
    sound_register = value;
}

void ChipEight::set_delay_register(std::uint8_t value) {
    delay_register = value;
}

void ChipEight::set_key(std::uint16_t pos, std::uint8_t value) {
    keys[pos] = value;
}

void ChipEight::set_is_waiting(bool value) {
    is_waiting = value;
}

void ChipEight::render_to_screen(std::uint32_t *pixels) {
    for (std::uint8_t x = 0; x < height; x++) {
        for (std::uint8_t y = 0; y < width; y++) {
            pixels[y * 64 + x] = display_memory[y * 64 + x];
        }
    }
}

void ChipEight::load_file(const char *filename) {
    std::ifstream file;

    file.open(filename, std::iostream::in | std::iostream::binary);
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();

    char *rom = (char*)(&(memory[0x200]));
    file.seekg(0, std::iostream::beg);
    file.read(rom, file_size);
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
    opcode = (memory[pc] << 0x8) | memory[pc + 1];
    std::uint16_t x = (opcode & 0x0F00) >> 0x8;
    std::uint16_t y = (opcode & 0x00F0) >> 0x4;
    std::uint8_t *vx = &v_registers[x & 0xF];
    std::uint8_t *vy = &v_registers[y & 0xF];
    std::uint8_t pc_inc = 2;

    switch(opcode & 0xF000) {
        case 0x0000:
            switch(opcode & 0x00FF) {
                case 0x00E0: // resets the display buffer
                    display_memory.fill(0);
                    break;
                case 0x00EE: // return from a subroutine
                    pc = stack[--sp];
                    break;
            }
            break;
        case 0x1000: // Set the program counter to 0nnn
            pc_inc = 0;
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: // Execute subroutine start at address NNN
            pc_inc = 0;
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: // Skip the following instruction if VX == NN
            if (*vx == (opcode & 0x00FF)) {
                pc_inc += 2;
            }
            break;
        case 0x4000: // Skip the following instruction if VX != NN
            if (*vx != (opcode & 0x00FF)) {
                pc_inc += 2;
            }
            break;
        case 0x5000: // Skip the following instruction if VX == VY
            if (*vx == *vy) {
                pc_inc += 2;
            }
            break;
        case 0x6000: // Sets VX = NN
            *vx = opcode & 0x00FF;
            break;
        case 0x7000: // Sets VX = VX + NN
            *vx += opcode & 0x00FF;
            break;
        case 0x8000: 
            switch (opcode & 0x000F) {
                case 0x0000: // Sets VX = VY
                    *vx = *vy;
                    break;
                case 0x0001: // Sets VX = VX | VY
                    *vx |= *vy;
                    break;
                case 0x0002: // Sets VX = VX & VY
                    *vx &= *vy;
                    break;
                case 0x0003: // Sets VX = VX ^ VY
                    *vx ^= *vy;
                    break;
                case 0x0004: { // Sets VX = VX + VY and VF as carry
                    if ((std::uint16_t)*vx + (std::uint16_t)*vy > 0xFF) {
                        v_registers[0xF] = 1;
                    } else {
                        v_registers[0xF] = 0;
                    }
                    *vx += *vy;
                    break;
                }
                case 0x0005: // Sets VX = VX - VY and VF as borrow
                    if (*vx > *vy) {
                        v_registers[0xF] = 1;
                    } else {
                        v_registers[0xF] = 0;
                    }
                    *vx -= *vy;
                    break;
                case 0x0006: // Shifts VX right by one and sets VF to the least significant bit
                    v_registers[0xF] = *vx & 0x1;
                    *vx = *vx >> 1;
                    break;
                case 0x0007: // Sets VX = VY - VX and VF as borrow
                    if (*vy > *vx) {
                        v_registers[0xF] = 1;
                    } else {
                        v_registers[0xF] = 0;
                    }
                    *vx = *vy - *vx;
                    break;
                case 0x000E: // Shifts VX left by one and sets VF to the most significant bit
                    if (*vx & 0x80) {
                        v_registers[0xF] = 1;
                    } else {
                        v_registers[0xF] = 0;
                    }
                    *vx = *vx << 1;
                    break;
            }
            break;
        case 0x9000: // Skip the following instruction if VX != VY
            if (*vx != *vy) {
                pc += 2;
            }
            break;
        case 0xA000: // Sets the I register to NNN
            i_register = opcode & 0x0FFF;
            break;
        case 0xB000: // Sets the program counter to NNN + V0
            pc = (opcode & 0x0FFF) + v_registers[0];
            break;
        case 0xC000: // Sets VX = (a random byte) & NN
            *vx = (get_random_byte() % 255) & (opcode & 0x00FF);
            break;
        case 0xD000: { //Display sprite
            std::uint16_t height = opcode & 0x000F;
            std::uint16_t pixel;
            v_registers[0xF] = 0;

            for (std::uint16_t y = 0; y < height; y++) {
                pixel = memory[i_register + y];
                for (std::uint16_t x = 0; x < 8; x++) {
                    if (pixel & (0x80 >> x)) {
                        if (display_memory[x+*vx+((y+*vy)*64)]) {
                            v_registers[0xF] = 1;
                        }
                        display_memory[x+*vx+((y+*vy)*64)] ^= 1;
                    }
                }
            }

            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF) {
                case (0x009E): // Skip the following instruction if the key pressed matches VX
                    if (keys[*vx]) {
                        pc += 2;
                    }
                    break;
                case (0x00A1): // Skip the following instruction if the key pressed doesn't match VX
                    if (!keys[*vx]) {
                        pc += 2;
                    }
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case (0x0007): // Store the current value of the delay register in VX
                    *vx = delay_register;
                    break;
                case (0x000A): // Wait for a keypress and store the result in VX
                    waiting_register = vx;
                    is_waiting = true;
                    break;
                case (0x0015): // Set the delay register to the value of VX
                    delay_register = *vx;
                    break;
                case (0x0018): // Set the sound register to the value of VX
                    sound_register = *vx;
                    break;
                case (0x001E): // Set the I register to I + VX
                    i_register += *vx;
                    break;
                case (0x0029):
                    // Maybe
                    i_register = *vx * 0x5;
                    break;
                case (0x0033):
                    // Maybe
                    memory[i_register] = *vx / 100;
                    memory[i_register+1] = (*vx / 10) % 10;
                    memory[i_register+2] = (*vx % 100) % 10;
                    break;
                case (0x0055):
                    // Maybe
                    for (std::uint8_t i = 0; i <= x; ++i) {
                        memory[i_register+i] = v_registers[i];
                    }
                    break;
                case (0x0065):
                    // Maybe
                    for (std::uint8_t i = 0; i <= x; ++i) {
                        v_registers[i] = memory[i_register + i];
                    }
                    break;
            }
        break;
    }
    pc += pc_inc;

    if (delay_register > 0) {
        delay_register--;
    }

    if (sound_register > 0) {
        sound_register--;
    }
}
