// instruction.h
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>

class Instruction {
public:
    std::string mnemonic;
    std::vector<std::string> operands;
    std::string opcode;

    Instruction(std::string mnem, std::vector<std::string> ops, std::string code)
        : mnemonic(mnem), operands(ops), opcode(code) {}
    std::string toString();
};

#endif
