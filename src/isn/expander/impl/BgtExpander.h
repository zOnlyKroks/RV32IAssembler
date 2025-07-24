#pragma once
#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BgtExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 3) {
            throw AssemblyException("bgt requires exactly 3 operands: bgt rs, rt, offset");
        }
        // bgt rs, rt, offset -> blt rt, rs, offset
        return { "blt " + operands[1] + ", " + operands[0] + ", " + operands[2] };
    }
};
