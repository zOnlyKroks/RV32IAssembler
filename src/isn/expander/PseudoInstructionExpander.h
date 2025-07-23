#ifndef PSEUDO_INSTRUCTION_EXPANDER_H
#define PSEUDO_INSTRUCTION_EXPANDER_H

#include <vector>
#include <string>

class PseudoInstructionExpander {
public:
    virtual ~PseudoInstructionExpander() = default;
    
    virtual std::vector<std::string> expand(const std::vector<std::string>& operands) = 0;
};

#endif // PSEUDO_INSTRUCTION_EXPANDER_H