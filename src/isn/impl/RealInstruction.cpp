#include "RealInstruction.h"

RealInstruction::RealInstruction(const std::string& mnemonic, 
                                 InstructionEncoder* encoder,
                                 const int opCode, const int funct3, const int funct7)
    : Instruction(mnemonic, encoder, opCode, funct3, funct7) {}