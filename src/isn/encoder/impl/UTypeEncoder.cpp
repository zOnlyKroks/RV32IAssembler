#include "UTypeEncoder.h"
#include <stdexcept>

uint32_t UTypeEncoder::encode(const std::string& mnemonic,
                              const std::vector<std::string>& operands,
                              uint32_t address,
                              const std::map<std::string, uint32_t>& labels) {
    if (operands.size() != 2) {
        throw std::runtime_error("Invalid operands for " + mnemonic);
    }

    const int rd = parseRegister(operands[0]);
    const int imm = parseImmediate(operands[1], 20, false);

    return (imm << 12) |
           (rd << 7) |
           getOpCode(mnemonic);
}