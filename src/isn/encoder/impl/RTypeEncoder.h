#ifndef R_TYPE_ENCODER_H
#define R_TYPE_ENCODER_H

#include "../InstructionEncoder.h"
#include "../Parser.h"
#include "../../../assembler/RV32IAssembler.h"

class RTypeEncoder final : public InstructionEncoder, public Parser {
public:
    uint32_t encode(const std::string& mnemonic,
                    const std::vector<std::string>& operands,
                    uint32_t address,
                    const std::map<std::string, uint32_t>& labels) override;
};

#endif // R_TYPE_ENCODER_H