#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>

#include "assembler/RV32IAssembler.h"

void exportLogisimV3_0(const std::vector<uint8_t>& bytes,
                       const std::string& outFilename) {
    std::ofstream outFile(outFilename);
    if (!outFile) {
        throw std::ios_base::failure(
            "Failed to open output file '" + outFilename + "'.");
    }

    outFile << "v3.0 hex words addressed\n";

    for (size_t addr = 0; addr < bytes.size(); addr += 16) {
        outFile << std::setw(6)
                << std::setfill('0')
                << std::hex
                << addr
                << ":";

        size_t bytesOnLine = std::min<size_t>(16, bytes.size() - addr);
        for (size_t i = 0; i < bytesOnLine; ++i) {
            outFile << " "
                    << std::setw(2)
                    << std::setfill('0')
                    << std::hex
                    << (bytes[addr + i] & 0xFF);
        }
        outFile << "\n";
    }
}

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [options]\n\n"
              << "Options:\n"
              << "  -i, --input <file>    Input assembly file (default: assembleMe.s)\n"
              << "  -o, --output <file>   Output Logisim v3 hex file (default: program_v3.hex)\n"
              << "  -h, --help            Show this help message\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile  = "assembleMe.s";
    std::string outputFile = "program_v3.hex";

    for (int i = 1; i < argc; ++i) {
        if (std::string arg = argv[i]; arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                inputFile = argv[++i];
            } else {
                std::cerr << arg << " requires a filename argument\n";
                return 1;
            }
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << arg << " requires a filename argument\n";
                return 1;
            }
        }
        else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    std::ifstream input(inputFile);
    if (!input) {
        std::cerr << "Error: cannot open input file '" << inputFile << "'\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    try {
        RV32IAssembler::init();
        std::vector<uint8_t> bytes = RV32IAssembler::assemble(lines);

        std::cout << "Assembly completed successfully!\n"
                  << "Generated " << bytes.size()
                  << " bytes (" << (bytes.size() / 4)
                  << " instructions)\n";

        exportLogisimV3_0(bytes, outputFile);
        std::cout << "Wrote Logisim v3 hex to '" << outputFile << "'\n";
    }
    catch (const AssemblyException& e) {
        std::cerr << "Assembly failed: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
