#ifndef BITTY_EMULATOR_H
#define BITTY_EMULATOR_H

#include <vector>
#include <cstdint>

class BittyEmulator {
public:
    BittyEmulator();
    uint16_t Evaluate(uint16_t instruction);
    uint16_t GetRegisterValue(uint16_t reg_num);
private:
    std::vector<uint16_t> registers_;
};

#endif
