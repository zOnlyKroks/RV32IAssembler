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
#include "../isn/expander/impl/BeqzExpander.h"
#include "../isn/expander/impl/LoadWordImmediateExpander.h"

std::map<std::string, std::unique_ptr<Instruction>> RV32IAssembler::instructions;
std::map<std::string, int> RV32IAssembler::registers;

RV32IAssembler::RV32IAssembler() {
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

    addPseudoInstruction("lwi", new LoadWordImmediateExpander());
    addPseudoInstruction("beqz", new BeqzExpander());
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
    int address = 0;

    for (const auto& line : lines) {
        std::string processed = removeComments(line);
        // Trim whitespace
        processed.erase(processed.begin(),
            std::find_if(processed.begin(), processed.end(),
                [](int ch) { return !std::isspace(ch); }));
        processed.erase(
            std::find_if(processed.rbegin(), processed.rend(),
                [](int ch) { return !std::isspace(ch); }).base(),
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
            std::vector<std::string> expanded = expandInstruction(processed);
            cleanedLines.insert(cleanedLines.end(), expanded.begin(), expanded.end());
            address += expanded.size() * 4;
        }
    }

    // Second pass: actual assembly
    address = 0;
    std::vector<uint8_t> output;
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

std::vector<std::string> RV32IAssembler::expandInstruction(const std::string& line) {
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
        return getPseudoInstruction(mnemonic)->getExpander()->expand(operands);
    } catch (const std::invalid_argument&) {
        return {line};
    }
}

std::string RV32IAssembler::removeComments(const std::string& line) {
    const size_t pos = line.find('#');
    return (pos != std::string::npos) ? line.substr(0, pos) : line;
}