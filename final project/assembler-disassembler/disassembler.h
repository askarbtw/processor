// disassembler.h
#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>

class Disassembler {
public:
    std::string disassembleLine(const std::string &binary);
};

#endif
