#pragma once

#include "../../../PseudoInstructionExpander.h"
#include "../../../../../assembler/RV32IAssembler.h"

class FSubImmExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                                    uint32_t address,
                                                    const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 3) {
            throw AssemblyException("fsubimm requires exactly 3 operands: fsubimm rd, imm1, imm2");
        }

        const std::string& rd = operands[0];
        const std::string& imm1 = operands[1];
        const std::string& imm2 = operands[2];

        const std::string helperImm3 = "1";
        const std::string helperImm4 = "0x01000000";

        return {
            "lwi x1, " + imm1,
            "lwi x2, " + imm2,
            "lwi x3, " + helperImm3,
            "lwi x4, " + helperImm4,
            "sw x1, 0(x4)",
            "sw x2, 1(x4)",
            "sw x3, 2(x4)",
            "lw " + rd + ", 3(x4)"
        };
    }
};