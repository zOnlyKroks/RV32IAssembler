#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BgtuExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 3) {
            throw AssemblyException("bgtu requires exactly 3 operands: bgtu rs, rt, offset");
        }
        // bgtu rs, rt, offset -> bltu rt, rs, offset
        return { "bltu " + operands[1] + ", " + operands[0] + ", " + operands[2] };
    }
};
