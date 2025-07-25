#ifndef RV32I_ASSEMBLER_H
#define RV32I_ASSEMBLER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cctype>
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
    static void init();

    static PseudoInstruction *getPseudoInstruction(const std::string &mnemonic);
    static RealInstruction *getRealInstruction(const std::string &mnemonic);

    static std::vector<uint8_t> assemble(const std::vector<std::string>& lines);

    static std::map<std::string, std::unique_ptr<Instruction>> instructions;
    static std::map<std::string, int> registers;
private:
    struct Binary {
        std::vector<uint8_t> text;
        std::vector<uint8_t> data;
        uint32_t dataBase;
    };

    static void initializeRegisters();
    static void initializeInstructions();
    static void addInstruction(const std::string& mnemonic, InstructionEncoder* encoder,
                               int opCode, int funct3, int funct7);
    static void addPseudoInstruction(const std::string& mnemonic,
                                     PseudoInstructionExpander* expander);

    static Binary assembleBinary(const std::vector<std::string>& lines);
    static std::vector<std::string> expandInstruction(const std::string& line,
                                                     uint32_t address,
                                                     const std::map<std::string, uint32_t>& labels,
                                                     int depth);
    static std::string removeComments(const std::string& line);

    static void trimInPlace(std::string &s) {
        s.erase(s.begin(), std::ranges::find_if(s, [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }
};

#endif // RV32I_ASSEMBLER_H