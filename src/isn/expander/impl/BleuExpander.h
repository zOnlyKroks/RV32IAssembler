#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BleuExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 3) {
            throw AssemblyException("bleu requires exactly 3 operands: bleu rs, rt, offset");
        }
        // bleu rs, rt, offset -> bgeu rt, rs, offset
        return { "bgeu " + operands[1] + ", " + operands[0] + ", " + operands[2] };
    }
};
