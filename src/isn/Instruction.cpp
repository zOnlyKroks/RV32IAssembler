#include "Instruction.h"

Instruction::Instruction(const std::string& mnemonic, 
                         InstructionEncoder* encoder,
                         const int opCode, const int funct3, const int funct7)
    : mnemonic(mnemonic), encoder(encoder), 
      opCode(opCode), funct3(funct3), funct7(funct7) {}

std::string Instruction::getMnemonic() const { 
    return mnemonic; 
}

InstructionEncoder* Instruction::getEncoder() const { 
    return encoder; 
}

int Instruction::getOpCode() const { 
    return opCode; 
}

int Instruction::getFunct3() const { 
    return funct3; 
}

int Instruction::getFunct7() const { 
    return funct7; 
}