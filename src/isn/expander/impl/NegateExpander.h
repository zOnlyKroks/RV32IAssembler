#include "../PseudoInstructionExpander.h"
#include "../../../assembler/RV32IAssembler.h"

class NegateExpander : public PseudoInstructionExpander {
public:
    std::vector<std::string> expand(const std::vector<std::string>& operands) override {
        if (operands.size() != 2) {
            throw AssemblyException("neg requires exactly 2 operands: neg rd, rs");
        }

        return {"sub " + operands[0] + ", zero, " + operands[1]};
    }
};
