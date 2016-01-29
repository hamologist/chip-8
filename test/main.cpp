#include <gtest/gtest.h>
#include "chip_eight.h"

class ChipEightTest : public ::testing::Test {
    protected:
        ChipEight chip_eight;
};

TEST_F(ChipEightTest, MemoryIs4096Bytes) {
    EXPECT_EQ(chip_eight.get_memory().size(), 4096);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
