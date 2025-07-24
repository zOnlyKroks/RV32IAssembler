#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class RetExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (!operands.empty()) {
            throw AssemblyException("ret requires no operands");
        }

        // ret => jalr x0, ra, 0
        return { "jalr x0, ra, 0" };
    }
};
