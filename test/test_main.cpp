#include "gtest/gtest.h"
#include "chip_eight.h"
#include <array>

class ChipEightTest : public ::testing::Test {
    protected:
        ChipEight chip_eight;
};

TEST_F(ChipEightTest, MemoryIs4096Bytes) {
    EXPECT_EQ(chip_eight.get_memory().size(), 4096);
}

TEST_F(ChipEightTest, Opcode00D0) {
    std::array<std::uint8_t, ChipEight::display_size> all_ones;
    std::array<std::uint8_t, ChipEight::display_size> all_zeroes;
    auto chip_eight_display = chip_eight.get_display_memory();

    all_ones.fill(1);
    all_zeroes.fill(0);

    chip_eight_display.fill(1);
    EXPECT_EQ(chip_eight_display, all_ones);

    chip_eight.set_i_register(0x00E0);
    chip_eight.execute_instruction();
    chip_eight_display = chip_eight.get_display_memory();
    EXPECT_EQ(chip_eight_display, all_zeroes);
}

TEST_F(ChipEightTest, Opcode1nnn) {
    auto pc = chip_eight.get_pc();

    EXPECT_EQ(pc, 0x0200);

    chip_eight.set_i_register(0x1234);
    chip_eight.execute_instruction();
    pc = chip_eight.get_pc();
    EXPECT_EQ(pc, 0x0234);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
