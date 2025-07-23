#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>

#include "assembler/RV32IAssembler.h"

void exportLogisimV3_0(const std::vector<uint8_t>& bytes) {
    std::ofstream outFile("program_v3.hex");
    if (!outFile) {
        throw std::ios_base::failure("Failed to open output file.");
    }

    outFile << "v3.0 hex words addressed\n";

    for (size_t addr = 0; addr < bytes.size(); addr += 16) {
        outFile << std::setw(6) << std::setfill('0') << std::hex << addr << ":";

        const size_t bytesOnLine = std::min<size_t>(16, bytes.size() - addr);
        for (size_t i = 0; i < bytesOnLine; ++i) {
            outFile << " " << std::setw(2) << std::setfill('0') << std::hex << (int)(bytes[addr + i] & 0xFF);
        }

        outFile << "\n";
    }
}

int main() {
    std::ifstream input("assembleMe.s");
    if (!input) {
        std::cerr << "File not found in working directory.\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    try {
        RV32IAssembler assembler;
        const std::vector<uint8_t> bytes = assembler.assemble(lines);

        std::cout << "Assembly completed successfully!\n";
        std::cout << "Generated " << bytes.size() << " bytes (" << (bytes.size() / 4) << " instructions)\n";

        exportLogisimV3_0(bytes);
    } catch (const AssemblyException& e) {
        std::cerr << "Assembly failed: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}