#include <iostream>
#include <cassert>
#include "BittyEmulator.h"

using namespace std;

int main() {
    BittyEmulator emulator;

    // Test 1: Load immediate value
    uint16_t loadInstruction = 0x1123; // Example instruction: opcode=0x1, reg=0x1, value=0x23
    emulator.Evaluate(loadInstruction);
	for (int i  = 0; i < 8; i++) { 
   cout << "first" << i << ": " << emulator.GetRegisterValue(i) << "\n";
}

uint16_t lInstruction = 0x1AE5;
uint16_t first = emulator.GetRegisterValue(0);
uint16_t second = emulator.GetRegisterValue(6);
uint16_t expected = first - second;
cout << "expected (reg0): " << expected << '\n';
emulator.Evaluate(lInstruction);
cout << "reg0: " << emulator.GetRegisterValue(0) << '\n';
cout << "reg6: " << emulator.GetRegisterValue(6) << '\n';
for (int i  = 0; i < 8; i++) { 
   cout << "second" << i << ": " << emulator.GetRegisterValue(i) << "\n";
}

    return 0;
}
