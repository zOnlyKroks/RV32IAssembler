#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include <map>
#include <stdexcept>
#include <utility>

class Parser {
public:
    virtual ~Parser() = default;

    static int parseRegister(const std::string& reg);

    static long long parseTerm(const std::string &term, const std::map<std::string, uint32_t> &labels);

    static long long parseSimpleExpression(const std::string &expr, const std::map<std::string, uint32_t> &labels);

    static int parseImmediate(const std::string &immStr, int bits, bool isSigned, const std::map<std::string, uint32_t> &labels);

    static std::pair<int, int> parseMemoryOperand(const std::string &mem, const std::map<std::string, uint32_t> &labels);

    static uint32_t parseConstant(const std::string &constStr, const std::map<std::string, uint32_t> &labels);
};

#endif // PARSER_H