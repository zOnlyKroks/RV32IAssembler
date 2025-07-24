#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class SnezExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("snez requires exactly 2 operands: snez rd, rs");
        }
        // snez rd, rs -> sltu rd, zero, rs
        return { "sltu " + operands[0] + ", zero, " + operands[1] };
    }
};
