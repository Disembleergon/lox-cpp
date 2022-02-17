#include "../include/parsing/Parser.h"
#include "../include/ErrorHandler.h"

using namespace lox;

Statement::stmt_vec Parser::parse()
{
    Statement::stmt_vec stmts; // shorthand for statements

    while (!isAtEnd())
    {
        stmts.emplace_back(statement());
    }

    return stmts;
}

////////////// private part ////////////////

// ----------- parse statements -------------

Statement::stmt_ptr Parser::statement()
{
    if (match({TokenType::PRINT}))
        return printStatement();

    return expressionStatement();
}

Statement::stmt_ptr Parser::expressionStatement()
{
    Expression::expr_ptr expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");

    return std::make_unique<ExpressionStatement>(ExpressionStatement{expr});
}

Statement::stmt_ptr Parser::printStatement()
{
    // after print comes an expression to print and a semicolon
    Expression::expr_ptr expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");

    return std::make_unique<PrintStatement>(PrintStatement{expr});
}

// ----------- parse expressions --------------

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
        return std::make_unique<Literal>(Literal{{false}});
    if (match({TRUE}))
        return std::make_unique<Literal>(Literal{{true}});
    if (match({NIL}))
        return std::make_unique<Literal>(Literal{{nullptr}});

    if (match({NUMBER, STRING}))
        return std::make_unique<Literal>(Literal{{previous().literal}});

    if (match({LEFT_PAREN}))
    {
        Expression::expr_ptr expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(Grouping{expr});
    }

    constexpr char message[] = "Expect expression.";
    ErrorHandler::error(peek(), message);
    throw std::runtime_error(message);
}

void lox::Parser::synchronize()
{
    advance();
    using enum TokenType;

    while (!isAtEnd())
    {
        if (previous().type == SEMICOLON)
            return;

        switch (peek().type)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }

        advance();
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

Token lox::Parser::consume(TokenType type, const std::string &&message)
{
    if (check(type))
        return advance();

    ErrorHandler::error(peek(), message);
    throw std::runtime_error{message};
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
