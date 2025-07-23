#include "InstructionEncoder.h"
#include "../../assembler/RV32IAssembler.h"

int InstructionEncoder::getFunct3(const std::string& mnemonic) {
    return RV32IAssembler::getRealInstruction(mnemonic)->getFunct3();
}

int InstructionEncoder::getFunct7(const std::string& mnemonic) {
    return RV32IAssembler::getRealInstruction(mnemonic)->getFunct7();
}

int InstructionEncoder::getOpCode(const std::string& mnemonic) {
    return RV32IAssembler::getRealInstruction(mnemonic)->getOpCode();
}