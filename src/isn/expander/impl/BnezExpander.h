#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BnezExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("bnez requires exactly 2 operands: bnez rs, label");
        }
        return { "bne " + operands[0] + ", x0, " + operands[1] };
    }
};
