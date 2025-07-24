#pragma once

#include "../../../PseudoInstructionExpander.h"
#include "../../../../../assembler/RV32IAssembler.h"

class FNegImmExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("fnegimm requires exactly 2 operands: faddimm rd, imm1");
        }

        const std::string& rd = operands[0];
        const std::string& imm1 = operands[1];

        const std::string helperImm3 = "4";
        const std::string helperImm4 = "0x01000000";

        return {
            "lwi x1, " + imm1,
            "lwi x2, " + helperImm3,
            "lwi x3, " + helperImm4,
            "sw x1, 0(x3)",
            "sw x2, 2(x3)",
            "lw " + rd + ", 3(x3)"
        };

    }
};