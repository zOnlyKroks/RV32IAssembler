#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class LoadAddressExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands,
                                                    uint32_t address,
                                                    const std::map<std::string, uint32_t>& labels) override {
        if (operands.size() != 2) {
            throw std::invalid_argument("la expects 2 operands: register, label");
        }

        const std::string& reg = operands[0];
        const std::string& label = operands[1];

        const auto labelIt = labels.find(label);
        if (labelIt == labels.end()) {
            throw std::invalid_argument("Undefined label: " + label);
        }

        const uint32_t labelAddress = labelIt->second;

        // Extract lower 12 bits as signed immediate
        const int32_t lower = static_cast<int32_t>(labelAddress << 20) >> 20;

        // Calculate upper 20 bits, adjusting for sign extension of lower bits
        const uint32_t upper = (labelAddress - static_cast<uint32_t>(lower)) >> 12;

        std::vector<std::string> result;

        if (upper != 0) {
            result.push_back("lui " + reg + ", " + std::to_string(upper));
            if (lower != 0) {
                result.push_back("addi " + reg + ", " + reg + ", " + std::to_string(lower));
            }
        } else {
            // If upper is 0, we can load the entire address with just addi
            result.push_back("addi " + reg + ", zero, " + std::to_string(lower));
        }

        return result;
    }
};