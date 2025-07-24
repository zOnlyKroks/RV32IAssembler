#ifndef TAIL_EXPANDER_H
#define TAIL_EXPANDER_H

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class TailExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 1) {
            throw AssemblyException("tail requires exactly 1 operand: tail label");
        }
        const std::string& label = operands[0];
        return {
            "auipc x6, %pcrel_hi(" + label + ")",
            "jalr x0, x6, %pcrel_lo(" + label + ")"
        };
    }
};

#endif // TAIL_EXPANDER_H
