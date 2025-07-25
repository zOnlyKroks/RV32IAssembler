#include "Parser.h"

#include <regex>
#include <stack>
#include <cmath>

#include "../../assembler/RV32IAssembler.h"

int Parser::parseRegister(const std::string& reg) {
    const auto it = RV32IAssembler::registers.find(reg);
    if (it == RV32IAssembler::registers.end()) {
        throw AssemblyException("Invalid register: " + reg);
    }
    return it->second;
}

long long evaluateExpression(const std::string& expr,
                            const std::map<std::string, uint32_t>& labels) {
    std::stack<long long> values;
    std::stack<char> ops;

    auto applyOp = [&]() {
        if (values.size() < 2) {
            throw AssemblyException("Invalid expression: insufficient operands");
        }
        if (ops.empty()) {
            throw AssemblyException("Invalid expression: no operator");
        }

        long long b = values.top(); values.pop();
        long long a = values.top(); values.pop();
        char op = ops.top(); ops.pop();

        switch(op) {
            case '+': values.push(a + b); break;
            case '-': values.push(a - b); break;
            case '*': values.push(a * b); break;
            case '/':
                if (b == 0) throw AssemblyException("Division by zero");
                values.push(a / b);
                break;
            case '&': values.push(a & b); break;
            case '|': values.push(a | b); break;
            case '^': values.push(a ^ b); break;
            case '<': values.push(a << b); break;
            case '>': values.push(a >> b); break;
            default:
                throw AssemblyException("Unknown operator: " + std::string(1, op));
        }
    };

    auto precedence = [](char op) {
        if (op == '*' || op == '/' || op == '&' || op == '|' || op == '^') return 2;
        if (op == '+' || op == '-') return 1;
        if (op == '<' || op == '>') return 3;
        return 0;
    };

    std::string token;
    bool expectOperand = true; // Track if we're expecting an operand or operator

    for (size_t i = 0; i < expr.size(); i++) {
        if (isspace(expr[i])) continue;

        if (expr[i] == '(') {
            ops.push('(');
            expectOperand = true;
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                applyOp();
            }
            if (ops.empty()) throw AssemblyException("Mismatched parentheses");
            ops.pop(); // Remove the '('
            expectOperand = false;
        } else if (isalnum(expr[i]) || expr[i] == '_' || expr[i] == '.') {
            // Parse number or identifier
            token.clear();
            while (i < expr.size() && (isalnum(expr[i]) || expr[i] == '_' || expr[i] == '.' || expr[i] == 'x')) {
                token += expr[i++];
            }
            i--; // Back up one since the loop will increment

            if (token.substr(0, 2) == "0x") {
                values.push(std::stoull(token.substr(2), nullptr, 16));
            } else if (isdigit(token[0])) {
                values.push(std::stoull(token, nullptr, 10));
            } else {
                auto it = labels.find(token);
                if (it == labels.end()) {
                    throw AssemblyException("Unknown label: " + token);
                }
                values.push(it->second);
            }
            expectOperand = false;
        } else if (strchr("+-*/&|^<>", expr[i])) {
            char op = expr[i];

            // Handle unary minus
            if (op == '-' && expectOperand) {
                // This is a unary minus, push 0 and treat as binary subtraction
                values.push(0);
                ops.push('-');
                expectOperand = true;
                continue;
            }

            // Handle shift operators (<<, >>)
            if ((op == '<' && i + 1 < expr.size() && expr[i + 1] == '<') ||
                (op == '>' && i + 1 < expr.size() && expr[i + 1] == '>')) {
                i++; // Skip the second character
            }

            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(op)) {
                applyOp();
            }
            ops.push(op);
            expectOperand = true;
        } else {
            throw AssemblyException("Invalid character in expression: " + std::string(1, expr[i]));
        }
    }

    while (!ops.empty()) {
        if (ops.top() == '(') {
            throw AssemblyException("Mismatched parentheses");
        }
        applyOp();
    }

    if (values.size() != 1) {
        throw AssemblyException("Invalid expression: " + expr);
    }

    return values.top();
}

int Parser::parseImmediate(const std::string& immStr, int bits, bool isSigned,
                          const std::map<std::string, uint32_t>& labels) {
    try {
        // Handle simple cases first to avoid expression parser
        if (immStr.empty()) {
            throw AssemblyException("Empty immediate value");
        }

        // Check if it's a simple number (decimal, hex, or negative)
        std::string trimmed = immStr;
        // Remove whitespace
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

        // Check if it's a simple number pattern
        std::regex simpleNumber(R"(^-?(0x[0-9a-fA-F]+|[0-9]+)$)");

        long long value;
        if (std::regex_match(trimmed, simpleNumber)) {
            // It's a simple number, parse directly
            if (trimmed.substr(0, 2) == "0x" || trimmed.substr(0, 3) == "-0x") {
                value = std::stoll(trimmed, nullptr, 16);
            } else {
                value = std::stoll(trimmed, nullptr, 10);
            }
        } else {
            // Check if it's just a label reference
            auto it = labels.find(trimmed);
            if (it != labels.end()) {
                value = static_cast<long long>(it->second);
            } else {
                // Use expression parser for complex expressions
                value = evaluateExpression(trimmed, labels);
            }
        }

        if (isSigned) {
            long long min = -(1LL << (bits - 1));
            long long max = (1LL << (bits - 1)) - 1;
            if (value < min || value > max) {
                throw AssemblyException("Immediate out of range: " + std::to_string(value) +
                    " (expected: " + std::to_string(min) + " to " + std::to_string(max) + ")");
            }
            return static_cast<int>(static_cast<uint32_t>(value) & ((1 << bits) - 1));
        } else {
            if (value < 0) throw AssemblyException("Negative value not allowed");
            if (static_cast<unsigned long long>(value) >= (1ULL << bits)) {
                throw AssemblyException("Immediate out of range: " + std::to_string(value) +
                    " (max: " + std::to_string((1ULL << bits) - 1) + ")");
            }
            return static_cast<int>(value);
        }
    } catch (const AssemblyException&) {
        throw; // Re-throw assembly exceptions as-is
    } catch (const std::exception& e) {
        throw AssemblyException("Invalid immediate: " + immStr + " (" + e.what() + ")");
    }
}

std::pair<int, int> Parser::parseMemoryOperand(const std::string& mem,
                                              const std::map<std::string, uint32_t>& labels) {
    // Match patterns like: [expression](reg), (reg), expression(reg)
    std::regex pattern(R"(\s*([^()]*?)\s*\(\s*(\w+)\s*\)\s*)");
    std::smatch match;

    if (!std::regex_match(mem, match, pattern)) {
        throw AssemblyException("Invalid memory operand: " + mem);
    }

    int offset = 0;
    if (match[1].matched && !match[1].str().empty()) {
        try {
            offset = static_cast<int>(evaluateExpression(match[1].str(), labels));
        } catch (const std::exception& e) {
            throw AssemblyException("Invalid offset in memory operand: " + mem + " (" + e.what() + ")");
        }
    }

    return {offset, parseRegister(match[2].str())};
}

uint32_t Parser::parseConstant(const std::string& constStr,
                              const std::map<std::string, uint32_t>& labels) {
    try {
        return static_cast<uint32_t>(evaluateExpression(constStr, labels));
    } catch (const std::exception& e) {
        throw AssemblyException("Invalid constant: " + constStr + " (" + e.what() + ")");
    }
}