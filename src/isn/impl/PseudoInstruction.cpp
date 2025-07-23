#include "PseudoInstruction.h"

PseudoInstruction::PseudoInstruction(const std::string& mnemonic, 
                                     PseudoInstructionExpander* expander)
    : Instruction(mnemonic, nullptr, 0, 0, 0), expander(expander) {}

PseudoInstructionExpander* PseudoInstruction::getExpander() const { 
    return expander; 
}