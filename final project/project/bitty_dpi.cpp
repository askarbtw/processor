#include <iostream>
#include "BittyEmulator.h"
    BittyEmulator emulator;

extern "C" void verify_instruction(uint16_t instruction, uint16_t hw_result) {
    uint16_t emu_result = emulator.Evaluate(instruction);

    std::cout << "Instruction: 0x" << std::hex << instruction 
              << " | Hardware result: 0x" << hw_result 
              << " | Emulator result: 0x" << emu_result << std::endl;

    if (hw_result != emu_result) {
        std::cerr << "Mismatch detected! Hardware result does not match emulator result." << std::endl;
    } else {
        std::cout << "Match confirmed. Hardware result matches emulator result." << std::endl;
    }
}
