#ifndef TAIL_EXPANDER_H
#define TAIL_EXPANDER_H

#include <vector>
#include <string>
#include <sstream>
#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class TailExpander final : public PseudoInstructionExpander {
public:
    // Basic expand method - throws exception since we need label context
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        throw AssemblyException("tail instruction requires label context - use expand with labels");
    }

    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                   uint32_t currentAddress,
                                   const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 1) {
            throw AssemblyException("tail requires exactly 1 operand: tail label");
        }

        const std::string& label = operands[0];

        auto it = labels.find(label);
        if (it == labels.end()) {
            throw AssemblyException("Label not found: " + label);
        }

        int32_t offset = static_cast<int32_t>(it->second) - static_cast<int32_t>(currentAddress);

        int32_t hi20 = (offset + 0x800) >> 12;
        int32_t lo12 = offset & 0xfff;
        if (lo12 & 0x800) lo12 |= 0xfffff000;

        std::ostringstream auipcInstr;
        auipcInstr << "auipc x6, " << hi20;

        std::ostringstream jalrInstr;
        jalrInstr << "jalr x0, x6, " << lo12;

        return {auipcInstr.str(), jalrInstr.str()};
    }

    [[nodiscard]] bool needsLabelContext() const override { return true; }
};

#endif // TAIL_EXPANDER_H