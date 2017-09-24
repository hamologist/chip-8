#include "chip_eight.h"
#include <iostream>
#include <cstdint>
#include <array>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(chip_eight) {
    class_<ChipEight>("ChipEight")
        .constructor<>()
        .function("execute_instruction", &ChipEight::execute_instruction)
        .function("load_file", optional_override(
            [](ChipEight& self, const std::string& s) {
                self.load_file(s.c_str());
            }
        ))
        .function("set_key", &ChipEight::set_key)
        .function("set_is_waiting", &ChipEight::set_is_waiting)
        .function("get_is_waiting", &ChipEight::get_is_waiting)
        .function("get_pc", &ChipEight::get_pc)
        .function("get_display_memory", optional_override(
            [](ChipEight& self) {
                return val(typed_memory_view(ChipEight::display_size, self.get_display_memory().data()));
            })
        )
        ;
}
