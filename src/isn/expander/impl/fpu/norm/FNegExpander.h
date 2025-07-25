#pragma once

#include "../../../PseudoInstructionExpander.h"
#include "../../../../../assembler/RV32IAssembler.h"

class FNegExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                                    uint32_t address,
                                                    const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 2) {
            throw AssemblyException("fneg requires exactly 2 operands: fneg rd, rs1");
        }

        const std::string& rd = operands[0];
        const std::string& rs1 = operands[1];

        const std::string helperImm3 = "4";
        const std::string helperImm4 = "0x01000000";

        return {
            "lwi x4, " + helperImm4,
            "sw " + rs1 + ", 0(x4)",
            "lwi x3, " + helperImm3,
            "sw x3, 2(x4)",
            "lw " + rd + ", 3(x4)"
        };
    }
};