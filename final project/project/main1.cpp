#include <iostream>
#include "Vbitt.h"
#include "BittyEmulator.h"
#include "BittyInstructionGenerator.h"
#include <vector>
#include <verilated.h>

using namespace std;

uint16_t g_hw_result = 0;

// Extracts the register index for RX from the instruction
int hwR(uint16_t instruction) {
    int rx_index = (instruction >> 13) & 0x7;
    return rx_index;
}

void execute_instruction(Vbitt* bitt, vluint64_t& sim_time, vluint64_t max_sim_time, bool& clk, uint16_t instruction) {
    // Your existing code here, no change needed
    // Decode Rx, Ry, and ALU select from the instruction
    uint16_t rx = (instruction >> 13) & 0x7;    // Extract bits 15-13 for Rx
    uint16_t ry = (instruction >> 10) & 0x7;    // Extract bits 12-10 for Ry
    uint16_t alu_sel = (instruction >> 2) & 0x7; // Extract bits 4-2 for ALU select
    std::cout << "The instruction: " << std::hex << instruction << '\n';
    vector<uint16_t> mo{bitt->mo0, bitt->mo1, bitt->mo2, bitt->mo3, bitt->mo4, bitt->mo5, bitt->mo6, bitt->mo7};
    uint16_t a = mo[rx];  // Use mo[rx] as the input 'a' for the ALU
    uint16_t b = mo[ry];
    std::cout << "First register: " << std::hex << mo[rx] << ", Register number: " << rx << "\n";
    std::cout << "Second register: " << std::hex << mo[ry] << ", Register number: " << ry << "\n";
    uint16_t expected_result;
    std::string operation;
    switch (alu_sel) {
        case 0: 
            expected_result = mo[rx] + mo[ry]; 
            operation = "Addition";
            break;
        case 1: 
            expected_result = mo[rx] - mo[ry]; 
            operation = "Subtraction";
            break;
        case 2: 
            expected_result = mo[rx] & mo[ry]; 
            operation = "AND";
            break;
        case 3: 
            expected_result = mo[rx] | mo[ry]; 
            operation = "OR";
            break;
        case 4: 
            expected_result = mo[rx] ^ mo[ry]; 
            operation = "XOR";
            break;
        case 5: 
            expected_result = mo[rx] << (mo[ry] & 0xF);
            operation = "Shift Left";
            break;
        case 6: 
            expected_result = mo[rx] >> (mo[ry] & 0xF);
            operation = "Shift Right";
            break;
        case 7:
            if (mo[rx] == mo[ry]) expected_result = 0;
            else if (mo[rx] > mo[ry]) expected_result = 1;
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
    std::cout << "Executing instruction: " << operation << " (Rx = " << rx << ", Ry = " << ry << ")" << std::endl;
    
    // Dump to VCD file in each clock cycle
    while (!bitt->done && sim_time < max_sim_time) {
        sim_time++;
        bitt->clk = clk;
        bitt->eval();
        clk = !clk;
    }
    // Continue with the rest of the function as in your existing code
}

void tick(Vbitt* dut, vluint64_t& sim_time) {
    dut->clk = 1;
    dut->eval();
    dut->clk = 0;
    dut->eval();
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
    bitt->mo7 = 0x0000;

    // First 20 cycles to stabilize reset
    while (sim_time < 20) {
        bitt->clk = clk;
        //bitt->eval();
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
    bool all_tests_passed = true;

    for (int i = 0; i < num_tests; i++) {
        uint16_t instruction = generator.Generate();
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
            std::cout << std::hex << "Emu: " << emulator.GetRegisterValue(i) << '\n';       
            std::cout << std::hex << "HW: " << vec[i] << '\n'; 
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

    return all_tests_passed ? 0 : 1;
}

