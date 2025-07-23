#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../isn/encoder/InstructionEncoder.h"

class Instruction {
public:
    Instruction(const std::string& mnemonic,
                InstructionEncoder* encoder,
                int opCode, int funct3, int funct7);

    virtual ~Instruction() = default;

    [[nodiscard]] std::string getMnemonic() const;
    [[nodiscard]] InstructionEncoder* getEncoder() const;
    [[nodiscard]] int getOpCode() const;
    [[nodiscard]] int getFunct3() const;
    [[nodiscard]] int getFunct7() const;

private:
    std::string mnemonic;
    InstructionEncoder* encoder;
    int opCode;
    int funct3;
    int funct7;
};

#endif // INSTRUCTION_H