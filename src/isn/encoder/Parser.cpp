#include "Parser.h"

#include <regex>

#include "../../assembler/RV32IAssembler.h"

int Parser::parseRegister(const std::string& reg) {
    const auto it = RV32IAssembler::registers.find(reg);
    if (it == RV32IAssembler::registers.end()) {
        throw AssemblyException("Invalid register: " + reg);
    }
    return it->second;
}

int Parser::parseImmediate(const std::string& immStr, const int bits, const bool isSigned) {
    if (bits > 32) {
        throw AssemblyException("Bit width too large: " + std::to_string(bits));
    }

    try {
        if (immStr.size() > 2 && immStr.substr(0, 2) == "0x") {
            const std::string hexStr = immStr.substr(2);
            char* end;
            const unsigned long value = std::strtoul(hexStr.c_str(), &end, 16);
            if (*end != '\0') {
                throw AssemblyException("Invalid hex immediate: " + immStr);
            }

            const unsigned long long masked = value & ((1ULL << bits) - 1);
            return static_cast<int>(masked);

        }

        char* end;
        const long long value = std::strtoll(immStr.c_str(), &end, 10);
        if (*end != '\0') {
            throw AssemblyException("Invalid immediate: " + immStr);
        }

        long long min, max;
        if (isSigned) {
            if (bits == 32) {
                min = std::numeric_limits<int32_t>::min();
                max = std::numeric_limits<int32_t>::max();
            } else {
                min = -(1LL << (bits - 1));
                max = (1LL << (bits - 1)) - 1;
            }
        } else {
            min = 0;
            if (bits == 32) {
                max = 0xFFFFFFFFLL;
            } else {
                max = (1LL << bits) - 1;
            }
        }

        if (value < min || value > max) {
            throw AssemblyException("Immediate out of range: " + immStr +
                " (expected range: " + std::to_string(min) + " to " +
                std::to_string(max) + ")");
        }

        return static_cast<int>(static_cast<uint32_t>(value) & ((1ULL << bits) - 1));
    }
    catch ([[maybe_unused]] const std::exception& e) {
        throw AssemblyException("Invalid immediate: " + immStr);
    }
}

std::pair<int, int> Parser::parseMemoryOperand(const std::string& mem) {
    const std::regex pattern(R"(([+-]?\d+)?\s*\(\s*(\w+)\s*\))");
    std::smatch match;

    if (!std::regex_match(mem, match, pattern)) {
        throw AssemblyException("Invalid memory operand: " + mem);
    }

    int offset = 0;
    if (match[1].matched) {
        try {
            offset = std::stoi(match[1].str());
        } catch (...) {
            throw AssemblyException("Invalid offset in memory operand: " + mem);
        }
    }

    return std::make_pair(offset, parseRegister(match[2].str()));
}
