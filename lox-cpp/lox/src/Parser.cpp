#include "../include/parsing/Parser.h"

using namespace lox;

Expression::expr_ptr Parser::expression()
{
    return equality();
}

Expression::expr_ptr Parser::equality()
{
    Expression::expr_ptr expr = comparison();

    using enum TokenType;
    while (match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        Token op = previous();
        Expression::expr_ptr right = comparison();
        expr = std::make_unique<Binary>(Binary{expr, op, right});
    }

    return expr;
}

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
