#include "../include/parsing/Parser.h"
#include "../include/ErrorHandler.h"

using namespace lox;

Statement::stmt_vec Parser::parse()
{
    Statement::stmt_vec stmts; // shorthand for statements

    while (!isAtEnd())
    {
        stmts.push_back(declaration()); // every line in the program is a declaration
    }

    return stmts;
}

////////////// private part ////////////////

// ----------- parse statements -------------

Statement::stmt_ptr lox::Parser::declaration()
{
    try
    {
        if (match(TokenType::VAR))
            return varDeclaration();

        return statement();
    }
    catch (std::runtime_error &e)
    {
        synchronize();
        return nullptr;
    }
}

Statement::stmt_ptr lox::Parser::varDeclaration()
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    Expression::expr_ptr initializer; // initializing is optional
    if (match(TokenType::EQUAL))
        initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStatement>(name, initializer);
}

Statement::stmt_vec lox::Parser::block()
{
    using TokenType::RIGHT_BRACE;

    Statement::stmt_vec stmts;
    while (!check(RIGHT_BRACE) && !isAtEnd())
        stmts.push_back(declaration());

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return stmts;
}

Statement::stmt_ptr Parser::statement()
{
    using enum TokenType;

    if (match(FOR))
        return forStatement();

    if (match(IF))
        return ifStatement();

    if (match(PRINT))
        return printStatement();

    if (match(WHILE))
        return whileStatement();

    if (match(BREAK))
    {
        consume(SEMICOLON, "Expect ';' after 'break'.");
        return std::make_shared<BreakStatement>();
    }

    if (match(LEFT_BRACE)) // block statement
    {
        Statement::stmt_vec stmts = block();
        return std::make_shared<BlockStatement>(stmts);
    }

    return expressionStatement();
}

Statement::stmt_ptr Parser::expressionStatement()
{
    Expression::expr_ptr expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");

    return std::make_shared<ExpressionStatement>(expr);
}

Statement::stmt_ptr lox::Parser::forStatement()
{
    using enum TokenType;
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Statement::stmt_ptr initializer;

    if (match(SEMICOLON)) // skip next semicolon, if it's there (skipping declaration)
        initializer = nullptr;
    else if (match(VAR))
        initializer = varDeclaration();
    else
        initializer = expressionStatement();

    Expression::expr_ptr condition;
    if (!check(SEMICOLON)) // user skips condition
        condition = expression();

    consume(SEMICOLON, "Expect ';' after loop condition.");

    Expression::expr_ptr increment;
    if (!check(RIGHT_PAREN)) // user skips increment
        increment = expression();

    consume(RIGHT_PAREN, "Expect ')' after clauses.");
    Statement::stmt_ptr body = statement();

    // ---- desugaring ----

    if (increment) // make var increment in while loop
    {
        // convert from expression to statement
        Statement::stmt_ptr incrementStmt = std::make_shared<ExpressionStatement>(increment);

        Statement::stmt_vec stmts;
        stmts.push_back(std::move(body));
        stmts.push_back(std::move(incrementStmt)); // increment at the end of all body statements (append to the rest)

        body = std::make_shared<BlockStatement>(stmts);
    }

    // default condition = true -> endless loop -> for (;;)
    if (!condition)
        condition = std::make_shared<LiteralExpression>(true);

    body = std::make_shared<WhileStatement>(condition, body);

    if (initializer)
    {
        Statement::stmt_vec stmts;
        stmts.push_back(std::move(initializer)); // append initializer before the while loop
        stmts.push_back(std::move(body));

        body = std::make_shared<BlockStatement>(stmts);
    }

    return body;
}

Statement::stmt_ptr lox::Parser::ifStatement()
{
    using enum TokenType;

    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expression::expr_ptr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Statement::stmt_ptr thenBranch = statement();
    Statement::stmt_ptr elseBranch; // nullptr by default

    if (match(ELSE))
        elseBranch = statement();

    return std::make_shared<IfStatement>(condition, thenBranch, elseBranch);
}

Statement::stmt_ptr Parser::printStatement()
{
    // after print comes an expression to print and a semicolon
    Expression::expr_ptr expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");

    return std::make_shared<PrintStatement>(expr);
}

Statement::stmt_ptr lox::Parser::whileStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    Expression::expr_ptr condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

    Statement::stmt_ptr body = statement();
    return std::make_shared<WhileStatement>(condition, body);
}

// ----------- parse expressions --------------

// evaluates to assignment(), just to prevent confusion
Expression::expr_ptr Parser::expression()
{
    return assignment();
}

Expression::expr_ptr lox::Parser::assignment()
{
    Expression::expr_ptr expr = orExpr();

    if (match(TokenType::EQUAL))
    {
        Token equals_op = previous();
        Expression::expr_ptr value = assignment();

        // if expr holds an AssignExpression (instanceof)
        if (VarExpression *var = dynamic_cast<VarExpression *>(expr.get()))
        {
            return std::make_shared<AssignExpression>(var->_name, value);
        }

        ErrorHandler::error(equals_op, "Invalid assignment target.");
    }

    return expr;
}

Expression::expr_ptr lox::Parser::orExpr()
{
    Expression::expr_ptr expr = andExpr();

    if (match(TokenType::OR))
    {
        Token op = previous();
        Expression::expr_ptr right = andExpr();
        return std::make_shared<LogicalExpression>(expr, op, right);
    }

    return expr;
}

Expression::expr_ptr lox::Parser::andExpr()
{
    Expression::expr_ptr expr = equality();

    if (match(TokenType::AND))
    {
        Token op = previous();
        Expression::expr_ptr right = equality();
        return std::make_shared<LogicalExpression>(expr, op, right);
    }

    return expr;
}

Expression::expr_ptr Parser::equality()
{
    Expression::expr_ptr expr = comparison();

    using enum TokenType;
    while (match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        Token op = previous();
        Expression::expr_ptr right = comparison();
        expr = std::make_shared<BinaryExpression>(expr, op, right);
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
        expr = std::make_shared<BinaryExpression>(expr, op, right);
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
        expr = std::make_shared<BinaryExpression>(expr, op, right);
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
        expr = std::make_shared<BinaryExpression>(expr, op, right);
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
        return std::make_shared<UnaryExpression>(op, right);
    }

    return call();
}

Expression::expr_ptr lox::Parser::call()
{
    Expression::expr_ptr expr = primary();

    for (;;)
    {
        if (match(TokenType::LEFT_PAREN))
            expr = finishCall(expr);
        else
            break;
    }

    return expr;
}

Expression::expr_ptr lox::Parser::primary()
{
    using enum TokenType;

    // ---- literals ----
    if (match(FALSE))
        return std::make_shared<LiteralExpression>(literal_t{false});
    if (match(TRUE))
        return std::make_shared<LiteralExpression>(literal_t{true});
    if (match(NIL))
        return std::make_shared<LiteralExpression>(literal_t{nullptr});

    if (match({NUMBER, STRING}))
        return std::make_shared<LiteralExpression>(literal_t{previous().literal});

    // variables
    if (match(IDENTIFIER))
        return std::make_shared<VarExpression>(previous());

    // grouping stuff
    if (match(LEFT_PAREN))
    {
        Expression::expr_ptr expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<GroupingExpression>(expr);
    }

    constexpr char message[] = "Expect expression.";
    ErrorHandler::error(peek(), message);
    throw std::runtime_error(message);
}

Expression::expr_ptr lox::Parser::finishCall(Expression::expr_ptr callee)
{
    Expression::expr_vec args;
    if (!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            args.push_back(std::move(expression()));
        } while (match(TokenType::COMMA));
    }

    const Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<CallExpression>(callee, paren, args);
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

bool lox::Parser::match(const TokenType &type)
{
    if (check(type))
    {
        advance();
        return true;
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

bool Parser::check(TokenType t) const
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
