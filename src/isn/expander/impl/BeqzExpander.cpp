#include "BeqzExpander.h"
#include "../../../assembler/RV32IAssembler.h"

std::vector<std::string> BeqzExpander::expand(const std::vector<std::string>& operands) {
    if (operands.size() != 2) {
        throw AssemblyException("beqz requires exactly 2 operands: beqz rs, label");
    }
    return {"beq " + operands[0] + ", x0, " + operands[1]};
}