#ifndef BITTY_INSTRUCTION_GENERATOR_H
#define BITTY_INSTRUCTION_GENERATOR_H

#include <cstdint>
#include <cstdlib>  // For rand()

class BittyInstructionGenerator {
public:
    BittyInstructionGenerator();
    uint16_t Generate();
};

#endif