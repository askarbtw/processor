#include "assembler.h"
#include "disassembler.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    std::string inputFile, outputFile;
    bool assemble = false, disassemble = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i") inputFile = argv[++i];
        else if (std::string(argv[i]) == "-o") outputFile = argv[++i];
        else if (std::string(argv[i]) == "-a") assemble = true;
        else if (std::string(argv[i]) == "-d") disassemble = true;
    }

    if (inputFile.empty() || (!assemble && !disassemble)) {
        std::cerr << "Error: Missing required arguments.\n";
        return 1;
    }

    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Cannot open input file.\n";
        return 1;
    }

    std::ofstream outFile;
    if (!outputFile.empty()) outFile.open(outputFile);

    Assembler assembler;
    Disassembler disassembler;

    std::string line;
    while (std::getline(inFile, line)) {
        std::string result = assemble ? assembler.assembleLine(line) : disassembler.disassembleLine(line);
        if (outFile.is_open()) outFile << result << "\n";
        else std::cout << result << "\n";
    }

    return 0;
}