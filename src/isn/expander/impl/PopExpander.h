#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class PopExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 1) {
            throw AssemblyException("pop requires exactly 1 operand: pop rd");
        }

        const std::string& reg = operands[0];

        return {
            "lw " + reg + ", 0(sp)",
            "addi sp, sp, 4"
        };
    }
};
