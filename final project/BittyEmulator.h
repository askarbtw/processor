#ifndef BITTY_EMULATOR_H
#define BITTY_EMULATOR_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <string>

class BittyEmulator {
public:
    BittyEmulator();
    uint16_t Evaluate(uint16_t instruction);
    uint16_t GetRegisterValue(uint16_t reg_num);

    void LoadInstructions(const std::string& filename);

    uint16_t FetchInstruction();

private:
    std::vector<uint16_t> registers_;
    uint16_t pc_;
    std::vector<uint16_t> memory_;
};

#endif
