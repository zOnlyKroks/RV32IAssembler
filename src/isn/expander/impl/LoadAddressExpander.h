#pragma once

#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class LoadAddressExpander final : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        throw AssemblyException("LoadAddressExpander requires label context");
    }
    
    std::vector<std::string> expand(const std::vector<std::string>& operands, 
                                   uint32_t address, 
                                   const std::map<std::string, uint32_t>& labels) override;
    
    [[nodiscard]] bool needsLabelContext() const override { return true; }
};
