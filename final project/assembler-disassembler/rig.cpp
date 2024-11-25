#include <iostream>
#include <fstream>
#include <bitset>  // For std::bitset
#include <cstdlib> // For std::atoi
#include "BittyInstructionGenerator.h"

int main(int argc, char* argv[]) {
    // Ensure correct usage
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <output_file>\n";
        std::cerr << "  N: Number of instructions to generate\n";
        std::cerr << "  output_file: File to save the instructions\n";
        return 1;
    }

    // Parse arguments
    int numInstructions = std::atoi(argv[1]);
    if (numInstructions <= 0) {
        std::cerr << "Error: N must be a positive integer.\n";
        return 1;
    }

    std::string outputFile = argv[2];

    // Open output file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open file " << outputFile << " for writing.\n";
        return 1;
    }

    // Generate instructions
    BittyInstructionGenerator generator;
    for (int i = 0; i < numInstructions; ++i) {
        uint16_t instruction = generator.Generate();
        outFile << std::bitset<16>(instruction) << std::endl; // Write in 16-bit binary format
    }

    std::cout << "Generated " << numInstructions << " instructions to " << outputFile << "\n";

    outFile.close();
    return 0;
}