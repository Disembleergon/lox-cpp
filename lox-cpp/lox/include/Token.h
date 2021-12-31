#ifndef TOKEN_H
#define TOKEN_H

#include <iomanip>
#include <iostream>
#include <variant>

#include "types/TokenType.h"

namespace lox
{

class Token
{
  public:
    // Literals can be strings or numbers (represented as double)
    using literal_t = std::variant<std::string, double>;

    const lox::TokenType type;
    const std::string lexeme;
    const literal_t literal;
    const int line;

    // make Token std::ostream compatible
    friend std::ostream &operator<<(std::ostream &stream, const Token &t)
    {
        // "convert" enum to string
        const auto token_type = lox::TokenType_Strings.at(t.type);

        // iomanip for tabular output
        stream << std::left << std::setw(15) << token_type; // token type
        stream << std::left << std::setw(15) << t.lexeme;   // lexeme

        // literal
        if (t.type == TokenType::STRING)
            stream << std::get<std::string>(t.literal);
        else if (t.type == TokenType::NUMBER)
            stream << std::get<double>(t.literal);

        return stream;
    }
};

} // namespace lox

#endif
