#include "../include/parsing/Parser.h"

using namespace lox;

Expression::expr_ptr Parser::expression()
{
    return equality();
}

Expression::expr_ptr Parser::equality(){Expression::expr_ptr expr = }

Expression::expr_ptr Parser::comparison()
{
    return Expression::expr_ptr();
}

bool Parser::match(const std::vector<TokenType> &types)
{
    for (const auto &type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType t)
{
    if (isAtEnd())
        return false;

    return peek().type == t;
}

Token Parser::advance()
{
    if (!isAtEnd())
        ++_current;

    return previous();
}
