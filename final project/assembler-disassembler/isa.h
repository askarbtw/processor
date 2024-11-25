// isa.h
#ifndef ISA_H
#define ISA_H

#include <string>
#include <unordered_map>

class ISA {
public:
    static std::unordered_map<std::string, std::string> mnemonicToOpcode;
    static std::unordered_map<std::string, std::string> opcodeToMnemonic;
    static std::string encodeRegister(const std::string &reg);
    static std::string decodeRegister(const std::string &bits);
};

#endif