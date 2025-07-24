#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class SeqzExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("seqz requires exactly 2 operands: seqz rd, rs");
        }
        return { "sltiu " + operands[0] + ", " + operands[1] + ", 1" };
    }
};
