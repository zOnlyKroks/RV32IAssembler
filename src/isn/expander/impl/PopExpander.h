#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class PopExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
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
