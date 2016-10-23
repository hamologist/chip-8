#include "chip_eight.h"
#include "chip_eight_display.h"
#include "chip_eight_controller.h"
#include <utility>

int main(int argc, char **argv) {
    ChipEight cpu;
    ChipEightDisplay display = ChipEightDisplay(&cpu);
    ChipEightController controller = ChipEightController(&cpu);

    cpu.load_file(argv[1]);

    while (!controller.esc_pressed()) {
        controller.process_input();
        cpu.execute_instruction();
        display.render_frame();
    }

    return 0;
}
