#include "BTypeEncoder.h"
#include <stdexcept>

uint32_t BTypeEncoder::encode(const std::string& mnemonic,
                              const std::vector<std::string>& operands,
                              uint32_t address,
                              const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 3) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rs1 = parseRegister(operands[0]);
    const int rs2 = parseRegister(operands[1]);
    const std::string label = operands[2];

    const auto labelIt = labels.find(label);
    if (labelIt == labels.end()) {
        throw std::runtime_error("Undefined label: " + label);
    }
    const uint32_t targetAddress = labelIt->second;

    const int32_t offset = static_cast<int32_t>(targetAddress - address);

    if ((offset & 0x1) != 0) {
        throw std::runtime_error("Unaligned branch target: " + label);
    }

    if (offset < -4096 || offset > 4094) {
        throw std::runtime_error("Branch offset out of range: " + std::to_string(offset));
    }

    const int32_t imm = offset >> 1;
    const int imm_12 = (imm >> 12) & 0x1;
    const int imm_11 = (imm >> 11) & 0x1;
    const int imm_10_5 = (imm >> 5) & 0x3F;
    const int imm_4_1 = imm & 0xF;
    const int funct3 = getFunct3(mnemonic);

    return (imm_12 << 31) |
           (imm_10_5 << 25) |
           (rs2 << 20) |
           (rs1 << 15) |
           (funct3 << 12) |
           (imm_4_1 << 8) |
           (imm_11 << 7) |
           getOpCode(mnemonic);
}