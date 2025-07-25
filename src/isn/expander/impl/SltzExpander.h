#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class SltzExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 2) {
            throw AssemblyException("sltz requires exactly 2 operands: sltz rd, rs");
        }
        // sltz rd, rs -> slt rd, rs, zero
        return { "slt " + operands[0] + ", " + operands[1] + ", zero" };
    }
};
