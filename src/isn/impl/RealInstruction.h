#ifndef REAL_INSTRUCTION_H
#define REAL_INSTRUCTION_H

#include "../Instruction.h"

class RealInstruction final : public Instruction {
public:
    RealInstruction(const std::string& mnemonic, 
                    InstructionEncoder* encoder,
                    int opCode, int funct3, int funct7);
};

#endif // REAL_INSTRUCTION_H