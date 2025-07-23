#ifndef LOAD_WORD_IMMEDIATE_EXPANDER_H
#define LOAD_WORD_IMMEDIATE_EXPANDER_H

#include "../PseudoInstructionExpander.h"
#include "../../encoder/Parser.h"
#include <vector>
#include <string>

class LoadWordImmediateExpander final : public PseudoInstructionExpander, public Parser {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override;
};

#endif // LOAD_WORD_IMMEDIATE_EXPANDER_H