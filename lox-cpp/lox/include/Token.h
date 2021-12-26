#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>

namespace lox
{
static enum class TokenType
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
  private:
    // Literals can be strings or numbers (represented as double)
    using LiteralType = std::variant<std::string, double>;

  public:
    const lox::TokenType type;
    const std::string lexeme;
    const LiteralType literal;
    const int line;
};

} // namespace lox

#endif