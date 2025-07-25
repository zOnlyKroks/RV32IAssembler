#include "ShiftITypeEncoder.h"
#include <stdexcept>

uint32_t ShiftITypeEncoder::encode(const std::string& mnemonic,
                                   const std::vector<std::string>& operands,
                                   uint32_t address,
                                   const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 3) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rd = parseRegister(operands[0]);
    const int rs1 = parseRegister(operands[1]);
    const int shamt = parseImmediate(operands[2], 5, false, labels);
    const int funct3 = getFunct3(mnemonic);
    const int funct7 = getFunct7(mnemonic);

    return (funct7 << 25) | 
           (shamt << 20) | 
           (rs1 << 15) | 
           (funct3 << 12) | 
           (rd << 7) | 
           getOpCode(mnemonic);
}