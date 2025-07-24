#pragma once
#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class NopExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (!operands.empty()) {
            throw AssemblyException("nop requires no operands");
        }
        // nop => addi x0, x0, 0
        return { "addi x0, x0, 0" };
    }
};
