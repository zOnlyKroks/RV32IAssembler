#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BlezExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("blez requires exactly 2 operands: blez rs, label");
        }
        const std::string& rs = operands[0];
        const std::string& label = operands[1];
        return { "bge " + rs + ", x0, " + label };
    }
};
