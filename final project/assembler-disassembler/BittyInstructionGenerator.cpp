#include "BittyInstructionGenerator.h"
#include <ctime>  // For seeding srand()

BittyInstructionGenerator::BittyInstructionGenerator() {
    // Seed the random number generator once
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

uint16_t BittyInstructionGenerator::Generate() {
    // Generate a 16-bit random instruction
    return static_cast<uint16_t>(std::rand() & 0xFFFF);  
}