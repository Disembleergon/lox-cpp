#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <variant>

namespace lox
{
const static enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    Eof
};

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
        const int token_type = static_cast<int>(t.type);

        stream << "type: " << token_type << " lexeme: " << t.lexeme; //<< " " << std::get(t.literal);
        return stream;
    }
};

} // namespace lox

#endif
