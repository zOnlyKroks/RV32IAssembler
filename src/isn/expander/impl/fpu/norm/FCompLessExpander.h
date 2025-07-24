#pragma once

#include "../../../PseudoInstructionExpander.h"
#include "../../../../../assembler/RV32IAssembler.h"

class FCompLessExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 3) {
            throw AssemblyException("fcmplt requires exactly 3 operands: fcmplt rd, rs1, rs2");
        }

        const std::string& rd = operands[0];
        const std::string& rs1 = operands[1];
        const std::string& rs2 = operands[2];

        const std::string helperImm3 = "7";
        const std::string helperImm4 = "0x01000000";

        return {
            "lwi x4, " + helperImm4,
            "sw " + rs1 + ", 0(x4)",
            "sw " + rs2 + ", 1(x4)",
            "lwi x3, " + helperImm3,
            "sw x3, 2(x4)",
            "lw " + rd + ", 3(x4)"
        };
    }
};