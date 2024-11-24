#include "BittyEmulator.h"
#include "BittyInstructionGenerator.h"
#include <ctime>
#include <iostream>

BittyEmulator::BittyEmulator() {
    registers_.resize(8, 0); // Initialize 8 registers with value 0
    registers_[0] = 0xE013;
    registers_[1] = 0x1234;
    registers_[2] = 0xF79A;
    registers_[3] = 0x8012;
    registers_[4] = 0x6345;
    registers_[5] = 0xABCD;
    registers_[6] = 0x7B23;
    registers_[7] = 0xABBB;
    
    /*for (int i = 0; i < 8; i++) {
        uint16_t instr = rand() & 0xFFFF;
        registers_[i] = instr;
    }*/
}

uint16_t BittyEmulator::Evaluate(uint16_t instruction) {
    // Decode Rx and Ry from instruction
    uint16_t rx_index = (instruction >> 13) & 0x7; // Bits 15-13
    uint16_t ry_index = (instruction >> 10) & 0x7; // Bits 12-10

    // Get ALU operation selector
    uint16_t alu_sel = (instruction >> 2) & 0x7; // Bits 4-2

    // Fetch register values
    uint16_t& rx = registers_[rx_index]; // Reference to Rx register for modification
    uint16_t ry = registers_[ry_index];  // Value of Ry register

    // Perform the operation based on ALU sel bits
    switch (alu_sel) {
        case 0x0: // Add
            rx = rx + ry;
            break;
        case 0x1: // Subtract
            rx = rx - ry;
            break;
        case 0x2: // Bitwise AND
            rx = rx & ry;
            break;
        case 0x3: // Bitwise OR
            rx = rx | ry;
            break;
        case 0x4: // Bitwise XOR
            rx = rx ^ ry;
            break;
        case 0x5: // Shift left
            rx = rx << (ry & 0xF);
            break;
        case 0x6: // Shift right
            rx = rx >> (ry & 0xF);
            break;
        case 0x7: // Compare
            if (rx == ry) {
                rx = 0;
            } else if (rx > ry) {
                rx = 1;
            } else {
                rx = 2;
            }
            break;
        default:
            break;
    }

    return rx; // Return the updated value of Rx for verification
}


uint16_t BittyEmulator::GetRegisterValue(uint16_t reg_num) {
    if (reg_num < registers_.size()) {
        return registers_[reg_num];
    }
    std::cout << "---------------ENTER REGISTERS 0-7 ONLY----------------\n";
    return 0;
}
