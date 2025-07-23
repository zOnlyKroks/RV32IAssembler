#include "LoadWordImmediateExpander.h"
#include <stdexcept>
#include <sstream>
#include "../../../assembler/RV32IAssembler.h"

std::vector<std::string> LoadWordImmediateExpander::expand(const std::vector<std::string>& operands) {
    if (operands.size() != 2) {
        throw new AssemblyException("lwi requires exactly 2 operands: lwi rd, imm");
    }

    const std::string& rd = operands[0];
    const std::string& immStr = operands[1];

    // Parse as 32-bit signed immediate (full range)
    int32_t imm = parseImmediate(immStr, 32, false);

    // Split into upper 20 and lower 12 bits
    int32_t lower12 = imm & 0xFFF;

    // Convert lower12 to hex string
    std::stringstream hexStream;
    hexStream << "0x" << std::hex << (lower12 & 0xFFF);
    std::string lower12Hex = hexStream.str();

    // Re-parse as 12-bit signed immediate to handle sign extension properly
    lower12 = parseImmediate(lower12Hex, 12, false);

    int32_t upper20 = static_cast<uint32_t>(imm) >> 12;

    std::vector<std::string> result;

    // Always add LUI instruction
    result.push_back("lui " + rd + ", " + std::to_string(upper20));

    // Only add addi if lower part is non-zero
    if (lower12 != 0) {
        result.push_back("addi " + rd + ", " + rd + ", " + lower12Hex);
    }

    return result;
}
