#ifndef BEQZ_EXPANDER_H
#define BEQZ_EXPANDER_H

#include "../PseudoInstructionExpander.h"

class BegzExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("beqz requires exactly 2 operands: beqz rs, label");
        }
        return {"beq " + operands[0] + ", x0, " + operands[1]};
    }
};

#endif // BEQZ_EXPANDER_H