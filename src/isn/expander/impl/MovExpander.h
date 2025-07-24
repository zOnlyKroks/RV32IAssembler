#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class MovExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("mov requires exactly 2 operands: mov rd, rs1");
        }
        return { "or " + operands[0] + ", " + operands[1] + ", x0" };
    }
};
