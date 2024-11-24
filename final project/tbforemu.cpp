#include "BittyEmulator.h"
#include <iostream>
#include <iomanip>
#include <bitset>

void printInstructionDetails(uint16_t instruction) {
    // Decode instruction components
    uint16_t rx = (instruction >> 13) & 0x7;  // Bits 15-13: Rx
    uint16_t ry = (instruction >> 10) & 0x7;  // Bits 12-10: Ry
    uint16_t alu_sel = (instruction >> 2) & 0x7; // Bits 4-2: ALU select
    uint16_t immediate = (instruction >> 5) & 0xFF; // Bits 12-5: Immediate value
    uint16_t format = instruction & 0x3; // Bits 1-0: Format
    
    // Determine instruction type
    std::cout << "Instruction: 0x" << std::hex << instruction << " | ";

    if (format == 0x1) {
        // Immediate instruction
        std::cout << "Type: Immediate | Rx: R" << rx << " | Immediate: 0x" 
                  << std::hex << immediate << std::endl;
    } else if (format == 0x2) {
        // Branch instruction
        uint16_t condition = (instruction >> 2) & 0x3; // Bits 3-2: Condition
        std::string condition_str;
        switch (condition) {
            case 0x0: condition_str = "BIE (Branch if Equal)"; break;
            case 0x1: condition_str = "BIG (Branch if Greater)"; break;
            case 0x2: condition_str = "BIL (Branch if Less)"; break;
            default: condition_str = "Unknown Condition"; break;
        }
        std::cout << "Type: Branch | Condition: " << condition_str
                  << " | Target Address: 0x" << std::hex << immediate << std::endl;
    } else {
        // ALU instruction
        std::string operation;
        switch (alu_sel) {
            case 0x0: operation = "Add"; break;
            case 0x1: operation = "Subtract"; break;
            case 0x2: operation = "AND"; break;
            case 0x3: operation = "OR"; break;
            case 0x4: operation = "XOR"; break;
            case 0x5: operation = "Shift Left"; break;
            case 0x6: operation = "Shift Right"; break;
            case 0x7: operation = "Compare"; break;
            default: operation = "Unknown ALU Operation"; break;
        }
        std::cout << "Type: ALU | Rx: R" << rx << " | Ry: R" << ry 
                  << " | Operation: " << operation << std::endl;
    }
}

int main() {
    try {
        BittyEmulator emulator;
        emulator.LoadInstructions("instructions.hex");
        size_t i = 0;
        while (i < 10) {
            // Fetch the next instruction
            uint16_t instruction = emulator.FetchInstruction();

            // Print decoded instruction details
            printInstructionDetails(instruction);

            // Evaluate the instruction
            uint16_t result = emulator.Evaluate(instruction);

            // Print register values after execution
            std::cout << "Register Values After Execution:" << std::endl;
            for (int i = 0; i < 8; i++) {
                std::cout << "R" << i << ": 0x" << std::hex << emulator.GetRegisterValue(i) << " ";
            }
            std::cout << std::endl;
            std::cout << "--------------------------------" << std::endl;
            i++;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
