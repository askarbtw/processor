// isa.cpp
#include "isa.h"
#include <bitset>

std::unordered_map<std::string, std::string> ISA::mnemonicToOpcode = {
    {"add", "000"}, {"sub", "001"}, {"and", "010"}, {"or", "011"},
    {"xor", "100"}, {"shl", "101"}, {"shr", "110"}, {"cmp", "111"},
    {"bie", "00"}, {"big", "01"}, {"bil", "10"}
};

std::unordered_map<std::string, std::string> ISA::opcodeToMnemonic = {
    {"000", "add"}, {"001", "sub"}, {"010", "and"}, {"011", "or"},
    {"100", "xor"}, {"101", "shl"}, {"110", "shr"}, {"111", "cmp"},
    {"00", "bie"}, {"01", "big"}, {"10", "bil"}
};

std::string ISA::encodeRegister(const std::string &reg) {
    return std::bitset<3>(std::stoi(reg.substr(1))).to_string();
}

std::string ISA::decodeRegister(const std::string &bits) {
    return "r" + std::to_string(std::stoi(bits, nullptr, 2));
}
