#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BltzExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 2) {
            throw AssemblyException("bltz requires exactly 2 operands: bltz rs, label");
        }
        const std::string& rs = operands[0];
        const std::string& label = operands[1];
        return { "bge " + rs + ", x0, " + label };
    }
};
