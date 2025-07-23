#include "JTypeEncoder.h"
#include <stdexcept>

uint32_t JTypeEncoder::encode(const std::string& mnemonic,
                              const std::vector<std::string>& operands,
                              uint32_t address,
                              const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 2) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rd = parseRegister(operands[0]);
    const std::string& label = operands[1];

    const auto labelIt = labels.find(label);
    if (labelIt == labels.end()) {
        throw std::runtime_error("Undefined label: " + label);
    }
    const uint32_t targetAddress = labelIt->second;

    const int32_t offset = static_cast<int32_t>(targetAddress) - static_cast<int32_t>(address);
    
    // Check alignment (must be 2-byte aligned)
    if ((offset & 0x1) != 0) {
        throw std::runtime_error("Unaligned jump target: " + label);
    }

    if (offset < -1048576 || offset > 1048574) {
        throw std::runtime_error("Jump offset out of range: " + std::to_string(offset));
    }

    const int32_t imm = offset >> 1;

    const uint32_t imm_20 = (imm >> 20) & 0x1;     // Bit 20
    const uint32_t imm_10_1 = imm & 0x3FF;          // Bits 10:1
    const uint32_t imm_11 = (imm >> 10) & 0x1;      // Bit 11
    const uint32_t imm_19_12 = (imm >> 11) & 0xFF;  // Bits 19:12

    return (imm_20 << 31) | 
           (imm_10_1 << 21) | 
           (imm_11 << 20) | 
           (imm_19_12 << 12) | 
           (rd << 7) | 
           getOpCode(mnemonic);
}