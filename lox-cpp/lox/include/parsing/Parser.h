#ifndef PARSER_H
#define PARSER_H

#include "../AST/Expressions.h"
#include <vector>

namespace lox
{

class Parser
{
  public:
    Parser(std::vector<Token> &tokens) : _tokens{std::move(tokens)}
    {
        // EMPTY
    }

  private:
    const std::vector<Token> _tokens;
    int _current{0};

    Expression::expr_ptr expression();
    Expression::expr_ptr equality();
    Expression::expr_ptr comparison();
    Expression::expr_ptr term();
    Expression::expr_ptr factor();
    Expression::expr_ptr unary();
    Expression::expr_ptr primary();

    // ---- helper functions ----

    // check if current token has any of the given types
    bool match(const std::vector<TokenType> &&);

    // returns true if the current token matches the given type
    bool check(TokenType);

    // consumes current token and returns it
    Token advance();

    Token peek()
    {
        return _tokens.at(_current);
    }

    Token previous()
    {
        return _tokens.at(_current - 1);
    }

    bool isAtEnd()
    {
        return peek().type == TokenType::Eof;
    }
};
} // namespace lox

#endif
