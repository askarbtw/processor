#include <iostream>
#include "Vbitt.h"
#include "BittyEmulator.h"
#include "BittyInstructionGenerator.h"
#include <vector>
#include <verilated.h>

using namespace std;

// Extracts the register index for RX from the instruction
int hwR(uint16_t instruction) {
    int rx_index = (instruction >> 13) & 0x7;
    return rx_index;
}

void execute_instruction(Vbitt* bitt, vluint64_t& sim_time, vluint64_t max_sim_time, bool& clk, uint16_t instruction) {
    // Decode Rx, Ry, ALU select, and immediate flag
    uint16_t rx = (instruction >> 13) & 0x7;    // Extract bits 15-13 for Rx
    uint16_t ry = (instruction >> 10) & 0x7;    // Extract bits 12-10 for Ry
    uint16_t alu_sel = (instruction >> 2) & 0x7; // Extract bits 4-2 for ALU select
    bool immediate_flag = (instruction & 0x3) == 0x1; // Immediate instruction flag
    uint16_t immediate_value = (instruction >> 5) & 0xFF; // Extract immediate value (bits 12-5)

    std::cout << "The instruction: " << std::hex << instruction << '\n';
    vector<uint16_t> mo{bitt->mo0, bitt->mo1, bitt->mo2, bitt->mo3, bitt->mo4, bitt->mo5, bitt->mo6, bitt->mo7};
    uint16_t a = mo[rx]; // Use mo[rx] as the input 'a' for the ALU
    uint16_t b = immediate_flag ? immediate_value : mo[ry]; // Use immediate value or mo[ry]

    std::cout << "First register: " << std::hex << a << ", Register number: " << rx << "\n";
    if (immediate_flag) {
        std::cout << "Immediate value: " << std::hex << immediate_value << "\n";
    } else {
        std::cout << "Second register: " << std::hex << mo[ry] << ", Register number: " << ry << "\n";
    }

    uint16_t expected_result;
    std::string operation;
    switch (alu_sel) {
        case 0: 
            expected_result = a + b; 
            operation = "Addition";
            break;
        case 1: 
            expected_result = a - b; 
            operation = "Subtraction";
            break;
        case 2: 
            expected_result = a & b; 
            operation = "AND";
            break;
        case 3: 
            expected_result = a | b; 
            operation = "OR";
            break;
        case 4: 
            expected_result = a ^ b; 
            operation = "XOR";
            break;
        case 5: 
            expected_result = a << (b & 0xF);
            operation = "Shift Left";
            break;
        case 6: 
            expected_result = a >> (b & 0xF);
            operation = "Shift Right";
            break;
        case 7:
            if (a == b) expected_result = 0;
            else if (a > b) expected_result = 1;
            else expected_result = 2;
            operation = "Comparison";
            break;
        default: 
            expected_result = 0;
            operation = "Unknown";
            break;
    }

    // Issue the instruction to the ALU
    bitt->instruction = instruction;
    std::cout << "Executing instruction: " << operation 
              << " (Rx = " << rx << ", " 
              << (immediate_flag ? "Immediate = " : "Ry = ") << b 
              << ")" << std::endl;
    
    // Wait for the `done` signal
    while (!bitt->done && sim_time < max_sim_time) {
        sim_time++;
        bitt->clk = clk;
        bitt->eval();
        clk = !clk;
    }

    // Verify the result
   /* if (bitt->mo0 == expected_result) {
        std::cout << "Result is correct: mo[rx] = " << std::hex << bitt->mo0 << std::endl;
    } else {
        std::cout << "Result is incorrect: mo[rx] = " << std::hex << bitt->mo0 
                  << " (expected: " << expected_result << ")" << std::endl;
    }*/

    // Wait for the `done` signal to go low
    while (bitt->done && sim_time < max_sim_time) {
        sim_time++;
        bitt->clk = clk;
        bitt->eval();
        clk = !clk;
    }
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vbitt* bitt = new Vbitt;

    vluint64_t sim_time = 0;
    vluint64_t max_sim_time = 300; // Maximum simulation time
    bool clk = false;

    // Initial global reset
    bitt->clk = 0;
    bitt->reset = 1;
    bitt->run = 0;
    bitt->instruction = 0x0000;

    // First 20 cycles to stabilize reset
    while (sim_time < 20) {
        bitt->clk = clk;
        bitt->eval();
        clk = !clk;
        sim_time++;
    }

    // De-assert reset
    bitt->reset = 0;
    bitt->run = 1;

    // Main simulation loop
    BittyEmulator emulator;
    BittyInstructionGenerator generator;

    const int num_tests = 5;
    vector<uint16_t> instructions_imm = {0xE3A5, 0x3BDD, 0xFDF1, 0xD6F5, 0x99B9}; 
    for (int i = 0; i < num_tests; i++) {
        // uint16_t instruction = generator.Generate();
        uint16_t instruction = instructions_imm[i];
        bitt->mo0 = emulator.GetRegisterValue(0);
        bitt->mo1 = emulator.GetRegisterValue(1);
        bitt->mo2 = emulator.GetRegisterValue(2);
        bitt->mo3 = emulator.GetRegisterValue(3);
        bitt->mo4 = emulator.GetRegisterValue(4);
        bitt->mo5 = emulator.GetRegisterValue(5);
        bitt->mo6 = emulator.GetRegisterValue(6);
        bitt->mo7 = emulator.GetRegisterValue(7);
        vector<uint16_t> vec{bitt->mo0, bitt->mo1, bitt->mo2, bitt->mo3, bitt->mo4, bitt->mo5, bitt->mo6, bitt->mo7};
        for (int i = 0; i < 8; i++) {
            std::cout << std::hex << "Em" << "[" << i << "]" << ": " << emulator.GetRegisterValue(i) << '\n';       
            std::cout << std::hex << "HW" << "[" << i << "]" << ": " << vec[i] << '\n'; 
        }
        
        uint16_t emu_res = emulator.Evaluate(instruction);
        bitt->instruction = instruction;
        
        if (sim_time >= 20) {
        execute_instruction(bitt, sim_time, max_sim_time, clk, instruction);
        while (bitt->done && sim_time < max_sim_time) {
            sim_time++;
            bitt->clk = clk;
            bitt->eval();
            clk = !clk;
        }
    }
        
    }
    
    delete bitt;

    return 0;
}
