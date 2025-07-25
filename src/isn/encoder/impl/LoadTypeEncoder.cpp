#include "LoadTypeEncoder.h"
#include <stdexcept>
#include <utility>

uint32_t LoadTypeEncoder::encode(const std::string& mnemonic,
                                 const std::vector<std::string>& operands,
                                 uint32_t address,
                                 const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 2) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rd = parseRegister(operands[0]);
    auto [offset, rs1] = parseMemoryOperand(operands[1], labels);
    const int funct3 = getFunct3(mnemonic);

    return (offset << 20) | 
           (rs1 << 15) | 
           (funct3 << 12) | 
           (rd << 7) | 
           getOpCode(mnemonic);
}