#ifndef PSEUDO_INSTRUCTION_EXPANDER_H
#define PSEUDO_INSTRUCTION_EXPANDER_H

#include <vector>
#include <string>
#include <map>

class PseudoInstructionExpander {
public:
    virtual ~PseudoInstructionExpander() = default;

    virtual std::vector<std::string> expand(const std::vector<std::string>& operands) = 0;

    virtual std::vector<std::string> expand(const std::vector<std::string>& operands,
                                           uint32_t currentAddress,
                                           const std::map<std::string, uint32_t>& labels) {
        return expand(operands);
    }

    [[nodiscard]] virtual bool needsLabelContext() const { return false; }
};

#endif // PSEUDO_INSTRUCTION_EXPANDER_H