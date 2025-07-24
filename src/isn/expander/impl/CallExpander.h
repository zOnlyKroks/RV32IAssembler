#ifndef CALL_EXPANDER_H
#define CALL_EXPANDER_H

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class CallExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("call requires exactly 2 operands: call rd, label");
        }
        const std::string& rd = operands[0];
        const std::string& label = operands[1];
        return {
            "auipc " + rd + ", %pcrel_hi(" + label + ")",
            "jalr " + rd + ", " + rd + ", %pcrel_lo(" + label + ")"
        };
    }
};

#endif // CALL_EXPANDER_H
