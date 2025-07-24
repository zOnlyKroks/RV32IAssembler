#ifndef JALRTYPE_ENCODER_H
#define JALRTYPE_ENCODER_H

#include "../InstructionEncoder.h"
#include "../Parser.h"

class JalrTypeEncoder final : public InstructionEncoder, public Parser {
public:
    uint32_t encode(const std::string& mnemonic,
                    const std::vector<std::string>& operands,
                    uint32_t address,
                    const std::map<std::string, uint32_t>& labels) override;
};

#endif // JALRTYPE_ENCODER_H