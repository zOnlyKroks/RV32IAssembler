#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class BleExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 3) {
            throw AssemblyException("ble requires exactly 3 operands: ble rs, rt, offset");
        }
        // ble rs, rt, offset -> bge rt, rs, offset
        return { "bge " + operands[1] + ", " + operands[0] + ", " + operands[2] };
    }
};
