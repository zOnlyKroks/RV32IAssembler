#include "RV32IAssembler.h"
#include <sstream>
#include <algorithm>
#include "../isn/expander/PseudoInstructionExpander.h"

#include "../isn/encoder/impl/BTypeEncoder.h"
#include "../isn/encoder/impl/ITypeEncoder.h"
#include "../isn/encoder/impl/JalrTypeEncoder.h"
#include "../isn/encoder/impl/JTypeEncoder.h"
#include "../isn/encoder/impl/LoadTypeEncoder.h"
#include "../isn/encoder/impl/RTypeEncoder.h"
#include "../isn/encoder/impl/ShiftITypeEncoder.h"
#include "../isn/encoder/impl/STypeEncoder.h"
#include "../isn/encoder/impl/UTypeEncoder.h"

#include "../isn/expander/impl/BegzExpander.h"
#include "../isn/expander/impl/BgezExpander.h"
#include "../isn/expander/impl/BgtExpander.h"
#include "../isn/expander/impl/BgtuExpander.h"
#include "../isn/expander/impl/BgtzExpander.h"
#include "../isn/expander/impl/BleExpander.h"
#include "../isn/expander/impl/BleuExpander.h"
#include "../isn/expander/impl/BlezExpander.h"
#include "../isn/expander/impl/BltzExpander.h"
#include "../isn/expander/impl/BnezExpander.h"
#include "../isn/expander/impl/JumpExpander.h"
#include "../isn/expander/impl/JumpRegisterExpander.h"
#include "../isn/expander/impl/MovExpander.h"
#include "../isn/expander/impl/NegateExpander.h"
#include "../isn/expander/impl/NopExpander.h"
#include "../isn/expander/impl/NotExpander.h"
#include "../isn/expander/impl/PopExpander.h"
#include "../isn/expander/impl/PushExpander.h"
#include "../isn/expander/impl/RetExpander.h"
#include "../isn/expander/impl/SeqzExpander.h"
#include "../isn/expander/impl/SgtzExpander.h"
#include "../isn/expander/impl/SltzExpander.h"
#include "../isn/expander/impl/SnezExpander.h"
#include "../isn/expander/impl/LoadWordImmediateExpander.h"

#include "../isn/expander/impl/fpu/FAddImmExpander.h"
#include "../isn/expander/impl/fpu/FSubImmExpander.h"
#include "../isn/expander/impl/fpu/FMultImmExpander.h"
#include "../isn/expander/impl/fpu/FDivImmExpander.h"
#include "../isn/expander/impl/fpu/FNegImmExpander.h"
#include "../isn/expander/impl/fpu/FCompAGreatB.h"
#include "../isn/expander/impl/fpu/FCompALessB.h"
#include "../isn/expander/impl/fpu/FCompAEqualB.h"
#include "../isn/expander/impl/fpu/F2IImmExpander.h"
#include "../isn/expander/impl/fpu/I2FImmExpander.h"

std::map<std::string, std::unique_ptr<Instruction>> RV32IAssembler::instructions;
std::map<std::string, int> RV32IAssembler::registers;

void RV32IAssembler::init() {
    static bool initialized = false;
    if (!initialized) {
        initializeRegisters();
        initializeInstructions();
        initialized = true;
    }
}

void RV32IAssembler::initializeRegisters() {
    for (int i = 0; i < 32; ++i) {
        const char* abiNames[] = {
            "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
            "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
            "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
            "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
        };
        registers["x" + std::to_string(i)] = i;
        registers[abiNames[i]] = i;
    }
}

void RV32IAssembler::initializeInstructions() {
    // R-type
    addInstruction("add", new RTypeEncoder(), 0x33, 0, 0x00);
    addInstruction("sub", new RTypeEncoder(), 0x33, 0, 0x20);
    addInstruction("xor", new RTypeEncoder(), 0x33, 4, 0x00);
    addInstruction("or", new RTypeEncoder(), 0x33, 6, 0x00);
    addInstruction("and", new RTypeEncoder(), 0x33, 7, 0x00);
    addInstruction("sll", new RTypeEncoder(), 0x33, 1, 0x00);
    addInstruction("srl", new RTypeEncoder(), 0x33, 5, 0x00);
    addInstruction("sra", new RTypeEncoder(), 0x33, 5, 0x20);
    addInstruction("slt", new RTypeEncoder(), 0x33, 2, 0x00);
    addInstruction("sltu", new RTypeEncoder(), 0x33, 3, 0x00);

    // I-type
    addInstruction("addi", new ITypeEncoder(), 0x13, 0, 0);
    addInstruction("xori", new ITypeEncoder(), 0x13, 4, 0);
    addInstruction("ori", new ITypeEncoder(), 0x13, 6, 0);
    addInstruction("andi", new ITypeEncoder(), 0x13, 7, 0);
    addInstruction("slti", new ITypeEncoder(), 0x13, 2, 0);
    addInstruction("sltiu", new ITypeEncoder(), 0x13, 3, 0);

    // Shift I-type
    addInstruction("slli", new ShiftITypeEncoder(), 0x13, 1, 0x00);
    addInstruction("srli", new ShiftITypeEncoder(), 0x13, 5, 0x00);
    addInstruction("srai", new ShiftITypeEncoder(), 0x13, 5, 0x20);

    // Load
    addInstruction("lb", new LoadTypeEncoder(), 0x03, 0, 0);
    addInstruction("lh", new LoadTypeEncoder(), 0x03, 1, 0);
    addInstruction("lw", new LoadTypeEncoder(), 0x03, 2, 0);
    addInstruction("lbu", new LoadTypeEncoder(), 0x03, 4, 0);
    addInstruction("lhu", new LoadTypeEncoder(), 0x03, 5, 0);

    // S-type
    addInstruction("sb", new STypeEncoder(), 0x23, 0, 0);
    addInstruction("sh", new STypeEncoder(), 0x23, 1, 0);
    addInstruction("sw", new STypeEncoder(), 0x23, 2, 0);

    // B-type
    addInstruction("beq", new BTypeEncoder(), 0x63, 0, 0);
    addInstruction("bne", new BTypeEncoder(), 0x63, 1, 0);
    addInstruction("blt", new BTypeEncoder(), 0x63, 4, 0);
    addInstruction("bge", new BTypeEncoder(), 0x63, 5, 0);
    addInstruction("bltu", new BTypeEncoder(), 0x63, 6, 0);
    addInstruction("bgeu", new BTypeEncoder(), 0x63, 7, 0);

    // J-type
    addInstruction("jal", new JTypeEncoder(), 0x6F, 0, 0);
    addInstruction("jalr", new JalrTypeEncoder(), 0x67, 0, 0);

    // U-type
    addInstruction("lui", new UTypeEncoder(), 0x37, 0, 0);
    addInstruction("auipc", new UTypeEncoder(), 0x17, 0, 0);

    // RV32M
    addInstruction("mul", new RTypeEncoder(), 0x33, 0, 0x01);
    addInstruction("mulh", new RTypeEncoder(), 0x33, 1, 0x01);
    addInstruction("mulhsu", new RTypeEncoder(), 0x33, 2, 0x01);
    addInstruction("mulhu", new RTypeEncoder(), 0x33, 3, 0x01);
    addInstruction("div", new RTypeEncoder(), 0x33, 4, 0x01);
    addInstruction("divu", new RTypeEncoder(), 0x33, 5, 0x01);
    addInstruction("rem", new RTypeEncoder(), 0x33, 6,0x01);
    addInstruction("remu", new RTypeEncoder(), 0x33, 7, 0x01);

    addPseudoInstruction("beqz", new BegzExpander());
    addPseudoInstruction("bgez", new BgezExpander());
    addPseudoInstruction("bgt", new BgtExpander());
    addPseudoInstruction("bgtu", new BgtuExpander());
    addPseudoInstruction("bgtz", new BgtzExpander());
    addPseudoInstruction("ble", new BleExpander());
    addPseudoInstruction("bleu", new BleuExpander());
    addPseudoInstruction("blez", new BlezExpander());
    addPseudoInstruction("bltz", new BltzExpander());
    addPseudoInstruction("bnez", new BnezExpander());
    addPseudoInstruction("j", new JumpExpander());
    addPseudoInstruction("jr", new JumpRegisterExpander());
    addPseudoInstruction("mov", new MovExpander());
    addPseudoInstruction("neg", new NegateExpander());
    addPseudoInstruction("nop", new NopExpander());
    addPseudoInstruction("not", new NotExpander());
    addPseudoInstruction("pop", new PopExpander());
    addPseudoInstruction("push", new PushExpander());
    addPseudoInstruction("ret", new RetExpander());
    addPseudoInstruction("seqz", new SeqzExpander());
    addPseudoInstruction("sgtz", new SgtzExpander());
    addPseudoInstruction("sltz", new SltzExpander());
    addPseudoInstruction("snez", new SnezExpander());
    addPseudoInstruction("lwi", new LoadWordImmediateExpander());

    addPseudoInstruction("faddimm", new FAddImmExpander());
    addPseudoInstruction("fsubimm", new FSubImmExpander());
    addPseudoInstruction("fmulimm", new FmulImmExpander());
    addPseudoInstruction("fdivimm", new FDivImmExpander());
    addPseudoInstruction("fnegimm", new FNegImmExpander());
    addPseudoInstruction("fcmpgt", new FCompAGreatBImmExpander());
    addPseudoInstruction("fcmplt", new FCompALessBImmExpander());
    addPseudoInstruction("fcmpeq", new FCompAEqualBImmExpander());

    addPseudoInstruction("f2iimm", new F2IImm());
    addPseudoInstruction("i2fimm", new I2FImm());
}

void RV32IAssembler::addInstruction(const std::string& mnemonic,
                                   InstructionEncoder* encoder,
                                   int opCode, int funct3, int funct7) {
    instructions[mnemonic] = std::make_unique<RealInstruction>(
        mnemonic, encoder, opCode, funct3, funct7
    );
}

void RV32IAssembler::addPseudoInstruction(const std::string& mnemonic,
                                         PseudoInstructionExpander* expander) {
    instructions[mnemonic] = std::make_unique<PseudoInstruction>(
        mnemonic, expander
    );
}

RealInstruction* RV32IAssembler::getRealInstruction(const std::string& mnemonic) {
    const auto it = instructions.find(mnemonic);
    if (it == instructions.end() || !dynamic_cast<RealInstruction*>(it->second.get())) {
        throw std::invalid_argument("No real instruction for: " + mnemonic);
    }
    return dynamic_cast<RealInstruction*>(it->second.get());
}

PseudoInstruction* RV32IAssembler::getPseudoInstruction(const std::string& mnemonic) {
    const auto it = instructions.find(mnemonic);
    if (it == instructions.end() || !dynamic_cast<PseudoInstruction*>(it->second.get())) {
        throw std::invalid_argument("No pseudo instruction for: " + mnemonic);
    }
    return dynamic_cast<PseudoInstruction*>(it->second.get());
}

std::vector<uint8_t> RV32IAssembler::assemble(const std::vector<std::string>& lines) {
    std::map<std::string, uint32_t> labels;
    std::vector<std::string> cleanedLines;
    uint32_t address = 0;

    for (const auto& line : lines) {
        std::string processed = removeComments(line);
        // Trim whitespace
        processed.erase(processed.begin(),
            std::ranges::find_if(processed,
                                 [](const int ch) { return !std::isspace(ch); }));
        processed.erase(
            std::find_if(processed.rbegin(), processed.rend(),
                [](const int ch) { return !std::isspace(ch); }).base(),
            processed.end());

        if (processed.empty()) continue;

        // Handle labels
        if (size_t colonPos = processed.find(':'); colonPos != std::string::npos) {
            if (std::string label = processed.substr(0, colonPos); !label.empty()) {
                if (labels.contains(label)) {
                    throw AssemblyException("Duplicate label: " + label);
                }
                labels[label] = address;
            }
            processed = processed.substr(colonPos + 1);
            // Trim again after removing label
            processed.erase(processed.begin(),
                std::ranges::find_if(processed,
                                     [](const int ch) { return !std::isspace(ch); }));
        }

        if (!processed.empty()) {
            std::vector<std::string> expanded = expandInstruction(processed, 0);
            cleanedLines.insert(cleanedLines.end(), expanded.begin(), expanded.end());
            address += static_cast<uint32_t>(expanded.size() * 4);
        }
    }

    address = 0;
    std::vector<uint8_t> output;
    output.reserve(cleanedLines.size() * 4);

    for (const auto& line : cleanedLines) {
        std::istringstream iss(line);
        std::string mnemonic;
        iss >> mnemonic;

        std::vector<std::string> operands;
        std::string token;
        while (std::getline(iss >> std::ws, token, ',')) {
            token.erase(token.begin(),
                std::ranges::find_if(token,
                                     [](const int ch) { return !std::isspace(ch); }));
            token.erase(
                std::find_if(token.rbegin(), token.rend(),
                    [](const int ch) { return !std::isspace(ch); }).base(),
                token.end());
            operands.push_back(token);
        }

        RealInstruction* inst = getRealInstruction(mnemonic);
        uint32_t encoded = inst->getEncoder()->encode(
            mnemonic, operands, address, labels
        );

        // Little-endian byte order
        output.push_back(encoded & 0xFF);
        output.push_back((encoded >> 8) & 0xFF);
        output.push_back((encoded >> 16) & 0xFF);
        output.push_back((encoded >> 24) & 0xFF);
        address += 4;
    }

    return output;
}

std::vector<std::string> RV32IAssembler::expandInstruction(const std::string& line, int depth) {
    if (depth > 10) {
        throw AssemblyException("Exceeded maximum recursion depth");
    }

    std::istringstream iss(line);
    std::string mnemonic;
    iss >> mnemonic;

    std::vector<std::string> operands;
    std::string token;
    while (std::getline(iss >> std::ws, token, ',')) {
        token.erase(token.begin(),
            std::ranges::find_if(token,
                                 [](const int ch) { return !std::isspace(ch); }));
        token.erase(
            std::find_if(token.rbegin(), token.rend(),
                [](const int ch) { return !std::isspace(ch); }).base(),
            token.end());
        operands.push_back(token);
    }

    try {
        PseudoInstruction* pseudo = getPseudoInstruction(mnemonic);
        std::vector<std::string> expanded = pseudo->getExpander()->expand(operands);

        std::vector<std::string> fullyExpanded;
        for (const auto& instr : expanded) {
            std::vector<std::string> recursed = expandInstruction(instr, depth + 1);
            fullyExpanded.insert(fullyExpanded.end(), recursed.begin(), recursed.end());
        }
        return fullyExpanded;
    } catch (const std::invalid_argument&) {
        return {line};
    }
}

std::string RV32IAssembler::removeComments(const std::string& line) {
    const size_t pos = line.find('#');
    return (pos != std::string::npos) ? line.substr(0, pos) : line;
}