#ifndef BEQZ_EXPANDER_H
#define BEQZ_EXPANDER_H

#include "../PseudoInstructionExpander.h"

class BeqzExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override;
};

#endif // BEQZ_EXPANDER_H