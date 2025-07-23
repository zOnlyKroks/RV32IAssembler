#include "LoadWordImmediateExpander.h"
#include <stdexcept>
#include <sstream>
#include "../../../assembler/RV32IAssembler.h"

std::vector<std::string> LoadWordImmediateExpander::expand(const std::vector<std::string>& operands) {
    if (operands.size() != 2) {
        throw AssemblyException("lwi requires exactly 2 operands: lwi rd, imm");
    }

    const std::string& rd     = operands[0];
    const std::string& immStr = operands[1];

    int32_t imm = parseImmediate(immStr, 32, false);

    int32_t lower12 = imm & 0xFFF;

    std::stringstream hexStream;
    hexStream << "0x" << std::hex << (lower12 & 0xFFF);
    const std::string lower12Hex = hexStream.str();

    lower12 = parseImmediate(lower12Hex, 12, false);

    const uint32_t upper20 = static_cast<uint32_t>(imm) >> 12;

    std::vector<std::string> result;

    if (lower12 != 0) {
        if (lower12 & 0x800) {
            result.push_back("lui " + rd + ", " + std::to_string(static_cast<int32_t>(upper20 ^ 0xFFFFF)));

            result.push_back("xori " + rd + ", " + rd + ", " + lower12Hex);
        } else {
            result.push_back("lui " + rd + ", " + std::to_string(static_cast<int32_t>(upper20)));

            result.push_back("ori " + rd + ", " + rd + ", " + lower12Hex);
        }
    } else {
        result.push_back("lui " + rd + ", " + std::to_string(static_cast<int32_t>(upper20)));
    }

    return result;
}
