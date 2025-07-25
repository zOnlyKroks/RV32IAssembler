#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class RetExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) override {
        if (!operands.empty()) {
            throw AssemblyException("ret requires no operands");
        }

        // ret => jalr x0, ra, 0
        return { "jalr x0, ra, 0" };
    }
};
