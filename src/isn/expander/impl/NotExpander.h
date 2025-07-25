#pragma once
#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class NotExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 2) {
            throw AssemblyException("not requires exactly 2 operands: not rd, rs");
        }
        return { "xori " + operands[0] + ", " + operands[1] + ", -1" };
    }
};
