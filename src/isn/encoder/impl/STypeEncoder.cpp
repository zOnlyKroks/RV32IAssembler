#include "STypeEncoder.h"
#include <stdexcept>
#include <utility>

uint32_t STypeEncoder::encode(const std::string& mnemonic,
                              const std::vector<std::string>& operands,
                              uint32_t address,
                              const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 2) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rs2 = parseRegister(operands[0]);
    auto [offset, rs1] = parseMemoryOperand(operands[1], labels);
    const int funct3 = getFunct3(mnemonic);

    const int imm_11_5 = (offset >> 5) & 0x7F;  // Bits 11-5
    const int imm_4_0 = offset & 0x1F;          // Bits 4-0

    return (imm_11_5 << 25) | 
           (rs2 << 20) | 
           (rs1 << 15) | 
           (funct3 << 12) | 
           (imm_4_0 << 7) | 
           getOpCode(mnemonic);
}