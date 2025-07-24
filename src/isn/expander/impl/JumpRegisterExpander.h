#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class JumpRegisterExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("jr requires exactly 2 operands: jr rs1, imm");
        }
        return { "jalr x0, " + operands[0] + ", " + operands[1] };
    }
};
