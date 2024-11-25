#include "disassembler.h"
#include "isa.h"
#include <bitset>
#include <stdexcept>
#include <sstream>

std::string Disassembler::disassembleLine(const std::string &binary) {
    std::bitset<16> instruction(binary);

    // Extract format bits (1-0)
    std::string format = binary.substr(14, 2);
    std::string opcode = binary.substr(11, 3);
    std::string mnemonic;
    if (format != "10") {
    // Extract opcode (bits 4-2)

    // Decode the mnemonic
    mnemonic = ISA::opcodeToMnemonic[opcode];
    if (format == "01") {
        mnemonic += "i"; // Immediate instruction
    }
    }
    // Decode operands
    std::ostringstream oss;
    oss << mnemonic << " ";
    if (format == "10") {
        std::string condition = binary.substr(12, 2);
        std::string mnemonic;
        if (condition == "00") mnemonic = "bie";
        else if (condition == "01") mnemonic = "big";
        else if (condition == "10") mnemonic = "bil";
        else mnemonic = "unknown";

        // Decode immediate value (11-0)
        int immValue = std::bitset<12>(binary.substr(0, 12)).to_ulong();

        std::ostringstream oss;
        oss << mnemonic << " " << immValue;
        return oss.str();
    } else if (format == "01") {
        // Immediate instruction: rx and immediate value
        std::string rx = ISA::decodeRegister(binary.substr(0, 3));
        int immValue = std::bitset<8>(binary.substr(3, 8)).to_ulong();
        oss << rx << ", " << immValue;
    } else if (format == "00") {
        // Standard instruction: rx and ry
        std::string rx = ISA::decodeRegister(binary.substr(0, 3));
        std::string ry = ISA::decodeRegister(binary.substr(3, 3));
        oss << rx << ", " << ry;
    } else {
        oss << "unknown"; // Handle invalid format bits gracefully
    }

    return oss.str();
}