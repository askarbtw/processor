#include "BittyEmulator.h"
#include "BittyInstructionGenerator.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

BittyEmulator::BittyEmulator() : pc_(0) {
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

    // Check for immediate or branch instruction flags
    bool immediate_flag = (instruction & 0x3) == 0x1;
    bool branch_flag = (instruction & 0x3) == 0x2;
    uint16_t immediate_value = (instruction >> 5) & 0xFF; // Bits 12-5

    // Fetch register values
    uint16_t& rx = registers_[rx_index]; // Reference to Rx register for modification
    uint16_t ry = immediate_flag ? immediate_value : registers_[ry_index]; // Use immediate value if applicable

    if (!branch_flag) {
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
                std::cerr << "Unknown ALU operation: " << alu_sel << std::endl;
                break;
        }
    } else {
        uint16_t cond = (instruction >> 2) & 0x3; // Bits 3-2 for condition
        switch (cond) {
            case 0x0:
                if (registers_[0] == 0) {
                    pc_ = immediate_value;
                }
                break;
            case 0x1:
                if (registers_[0] > 0) {
                    pc_ = immediate_value;
                }
                break;
            case 0x2:
                if (registers_[0] < 0) {
                    pc_ = immediate_value;
                }
                break;
            default:
                std::cerr << "Unknown branch condition: " << cond << std::endl;
                break;
        }
    }

    return rx;
}


uint16_t BittyEmulator::GetRegisterValue(uint16_t reg_num) {
    if (reg_num < registers_.size()) {
        return registers_[reg_num];
    }
    std::cerr << "Error: Invalid register number. Enter registers 0-7 only.\n";
    return 0;
}

void BittyEmulator::LoadInstructions(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open instructions file: " + filename);
    }

    memory_.clear(); // Clear any existing instructions
    std::string line;
    while (std::getline(file, line)) {
        uint16_t instruction;
        std::istringstream iss(line);
        iss >> std::hex >> instruction; // Read hex value
        memory_.push_back(instruction);
    }

    file.close();
}

uint16_t BittyEmulator::FetchInstruction() {
    if (pc_ >= memory_.size()) {
        throw std::runtime_error("Error: Program counter out of bounds.");
    }
    return memory_[pc_++];
}



