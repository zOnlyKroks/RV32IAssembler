#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class SgtzExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("sgtz requires exactly 2 operands: sgtz rd, rs");
        }
        // sgtz rd, rs -> slt rd, zero, rs
        return { "slt " + operands[0] + ", zero, " + operands[1] };
    }
};
