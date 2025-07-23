#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <map>
#include <stdexcept>
#include <utility>
class Parser {
public:
    virtual ~Parser() = default;

    static int parseRegister(const std::string& reg);

    static int parseImmediate(const std::string& immStr, int bits, bool isSigned);

    static std::pair<int, int> parseMemoryOperand(const std::string& mem);
};

#endif // PARSER_H