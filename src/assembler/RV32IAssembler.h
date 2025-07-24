#ifndef RV32I_ASSEMBLER_H
#define RV32I_ASSEMBLER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "../isn/Instruction.h"
#include "../isn/impl/PseudoInstruction.h"
#include "../isn/impl/RealInstruction.h"


class AssemblyException final : public std::runtime_error {
public:
    explicit AssemblyException(const std::string& message)
        : std::runtime_error(message) {}
};

class RV32IAssembler {
public:
    static std::map<std::string, std::unique_ptr<Instruction>> instructions;
    static std::map<std::string, int> registers;

    static RealInstruction* getRealInstruction(const std::string& mnemonic);
    static PseudoInstruction* getPseudoInstruction(const std::string& mnemonic);

    static std::vector<uint8_t> assemble(const std::vector<std::string>& lines);
    static void init();

private:
    static void initializeRegisters();

    static void initializeInstructions();

    static void addInstruction(const std::string& mnemonic, InstructionEncoder* encoder,
                               int opCode, int funct3, int funct7);

    static void addPseudoInstruction(const std::string& mnemonic,
                                     PseudoInstructionExpander* expander);

    static std::vector<std::string> expandInstruction(const std::string& line, int depth);
    static std::string removeComments(const std::string& line);
};

#endif // RV32I_ASSEMBLER_H