#include "assembler.h"
#include "isa.h"
#include <sstream>
#include <bitset>
#include <unordered_map>
#include <stdexcept>

std::string Assembler::assembleLine(const std::string &line) {
    std::istringstream iss(line);
    std::string mnemonic, result;
    iss >> mnemonic;
    bool isBranch = (mnemonic == "bie" || mnemonic == "big" || mnemonic == "bil");

    // Determine if it's an immediate instruction (ends with 'i')
    bool isImmediate = mnemonic.back() == 'i';
    if (isImmediate) {
        mnemonic.pop_back(); // Remove the 'i' for lookup
    }

    // Fetch opcode
    if (ISA::mnemonicToOpcode.find(mnemonic) == ISA::mnemonicToOpcode.end()) {
        throw std::invalid_argument("Invalid mnemonic: " + mnemonic);
    }
    std::string opcode = ISA::mnemonicToOpcode[mnemonic];

    // Create a 16-bit instruction
    std::bitset<16> instruction;

    // Set opcode (bits 4-2)
    instruction.set(11, opcode[0] - '0');
    instruction.set(12, opcode[1] - '0');
    instruction.set(13, opcode[2] - '0');

    // Set format bits (1-0)
    if (isBranch) {
        instruction.set(14, 1);
        instruction.set(15, 0);
    } else if (isImmediate) {
        instruction.set(14, 0);
        instruction.set(15, 1); // Format for immediate
    } else {
        instruction.set(14, 0);
        instruction.set(15, 0); // Format for standard instruction
    }

    // Process operands
    
    if (isBranch) {
        // Set branch condition (bits 13-12)
        std::string conditionBits;
        if (mnemonic == "bie") conditionBits = "00";
        else if (mnemonic == "big") conditionBits = "01";
        else if (mnemonic == "bil") conditionBits = "10";

        instruction.set(12, conditionBits[0] - '0');
        instruction.set(13, conditionBits[1] - '0');

        // Parse immediate value
        std::string immediate;
        iss >> immediate;
        std::bitset<12> immBits = std::bitset<12>(std::stoi(immediate));
        for (int i = 0; i < 12; ++i) {
            instruction.set(i, immBits[11 - i]);
        }
    } else if (isImmediate) {
        // Immediate instruction: rx and immediate value
        std::string rx, immValue;
        iss >> rx >> immValue;

        // Set rx (bits 15-13)
        std::bitset<3> regBits = std::bitset<3>(std::stoi(rx.substr(1)));
        for (int i = 0; i < 3; ++i) {
            instruction.set(i, regBits[2 - i]);
        }

        // Set immediate value (bits 3-10)
        std::bitset<8> immBits = std::bitset<8>(std::stoi(immValue));
        for (int i = 0; i < 8; ++i) {
            instruction.set(3 + i, immBits[7 - i]);
        }
    } else {
        // Standard instruction: rx and ry
        std::string rx, ry;
        iss >> rx >> ry;

        // Set rx (bits 15-13)
        std::bitset<3> regBitsRx = std::bitset<3>(std::stoi(rx.substr(1)));
        for (int i = 0; i < 3; ++i) {
            instruction.set(i, regBitsRx[2 - i]);
        }

        // Set ry (bits 3-5)
        std::bitset<3> regBitsRy = std::bitset<3>(std::stoi(ry.substr(1)));
        for (int i = 0; i < 3; ++i) {
            instruction.set(3 + i, regBitsRy[2 - i]);
        }
    }

    // Convert to string and return
    result = instruction.to_string();
    std::reverse(result.begin(), result.end());
    return result;
}