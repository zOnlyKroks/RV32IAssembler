#ifndef PSEUDO_INSTRUCTION_H
#define PSEUDO_INSTRUCTION_H

#include <string>
#include "../../isn/Instruction.h"
#include "../expander/PseudoInstructionExpander.h"

class PseudoInstruction final : public Instruction {
public:
    PseudoInstruction(const std::string& mnemonic, 
                      PseudoInstructionExpander* expander);
    
    [[nodiscard]] PseudoInstructionExpander* getExpander() const;

private:
    PseudoInstructionExpander* expander;
};

#endif // PSEUDO_INSTRUCTION_H