#include "../include/parsing/Parser.h"

using namespace lox;

// evaluates to equality(), just to prevent confusion
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
    Expression::expr_ptr expr = term();

    using enum TokenType;
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}))
    {
        Token op = previous();
        Expression::expr_ptr right = term();
        expr = std::make_unique<Binary>(Binary{expr, op, right});
    }

    return expr;
}

Expression::expr_ptr lox::Parser::term()
{
    Expression::expr_ptr expr = factor();

    using enum TokenType;
    while (match({MINUS, PLUS}))
    {
        Token op = previous();
        Expression::expr_ptr right = factor();
        expr = std::make_unique<Binary>(Binary{expr, op, right});
    }

    return expr;
}

Expression::expr_ptr lox::Parser::factor()
{
    Expression::expr_ptr expr = unary();

    using enum TokenType;
    while (match({SLASH, STAR}))
    {
        Token op = previous();
        Expression::expr_ptr right = unary();
        expr = std::make_unique<Binary>(Binary{expr, op, right});
    }

    return expr;
}

Expression::expr_ptr lox::Parser::unary()
{
    using enum TokenType;
    if (match({BANG, MINUS}))
    {
        Token op = previous();
        Expression::expr_ptr right = unary();
        return std::make_unique<Unary>(Unary{op, right});
    }

    return primary();
}

Expression::expr_ptr lox::Parser::primary()
{
    using enum TokenType;
    if (match({FALSE}))
        return std::make_unique<Literal>(Literal{{"false"}});
    if (match({TRUE}))
        return std::make_unique<Literal>(Literal{{"true"}});
    if (match({NIL}))
        return std::make_unique<Literal>(Literal{{"nil"}});

    if (match({NUMBER, STRING}))
        return std::make_unique<Literal>(Literal{{previous().literal}});

    if (match({LEFT_PAREN}))
    {
        Expression::expr_ptr expr = expression();
        // consume(RIGHT_PAREN, "Expect ')' after expression.");
        // return std::make_unique<Grouping>(Grouping{expr});
    }
}

bool Parser::match(const std::vector<TokenType> &&types)
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
