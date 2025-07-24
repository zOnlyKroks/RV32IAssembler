#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BgtzExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("bgtz requires exactly 2 operands: bgtz rs, label");
        }
        std::string rs = operands[0];
        std::string label = operands[1];
        return {"blt x0, " + rs + ", " + label};
    }
};
