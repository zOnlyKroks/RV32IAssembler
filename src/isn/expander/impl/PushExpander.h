#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class PushExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 1) {
            throw AssemblyException("push requires exactly 1 operand: push rs");
        }

        const std::string& reg = operands[0];

        return {
            "addi sp, sp, -4",
            "sw " + reg + ", 0(sp)"
        };
    }
};
