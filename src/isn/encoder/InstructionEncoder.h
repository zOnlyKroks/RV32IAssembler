#ifndef INSTRUCTION_ENCODER_H
#define INSTRUCTION_ENCODER_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>

class InstructionEncoder {
public:
    virtual ~InstructionEncoder() = default;

    virtual uint32_t encode(const std::string& mnemonic,
                            const std::vector<std::string>& operands,
                            uint32_t address,
                            const std::map<std::string, uint32_t>& labels) = 0;

    static int getFunct3(const std::string& mnemonic);
    static int getFunct7(const std::string& mnemonic);
    static int getOpCode(const std::string& mnemonic);
};

#endif // INSTRUCTION_ENCODER_H