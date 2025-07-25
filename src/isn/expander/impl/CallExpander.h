#ifndef CALL_EXPANDER_H
#define CALL_EXPANDER_H

#include <vector>
#include <string>
#include <sstream>
#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class CallExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                   const uint32_t currentAddress,
                                   const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 1) {
            throw AssemblyException("call requires exactly 1 operand: call label");
        }

        const std::string& label = operands[0];

        const auto it = labels.find(label);
        if (it == labels.end()) {
            throw AssemblyException("Label not found: " + label);
        }

        const int32_t offset = static_cast<int32_t>(it->second) - static_cast<int32_t>(currentAddress);

        const int32_t hi20 = (offset + 0x800) >> 12;
        int32_t lo12 = offset & 0xfff;
        if (lo12 & 0x800) lo12 |= 0xfffff000;

        std::ostringstream auipcInstr;
        auipcInstr << "auipc ra, " << hi20;

        std::ostringstream jalrInstr;
        jalrInstr << "jalr ra, ra, " << lo12;

        return {auipcInstr.str(), jalrInstr.str()};
    }
};

#endif // CALL_EXPANDER_H