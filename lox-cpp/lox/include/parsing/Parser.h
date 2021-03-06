#ifndef PARSER_H
#define PARSER_H

#include "../AST/Expressions.h"
#include "../AST/Statements.h"
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

    Statement::stmt_vec parse();

  protected:
    // statement parsing
    Statement::stmt_ptr declaration();
    Statement::stmt_ptr varDeclaration();
    Statement::stmt_ptr function();
    Statement::stmt_vec block(); // returns all the statements in the block
    Statement::stmt_ptr statement();
    Statement::stmt_ptr expressionStatement();
    Statement::stmt_ptr forStatement();
    Statement::stmt_ptr ifStatement();
    Statement::stmt_ptr printStatement();
    Statement::stmt_ptr returnStatement();
    Statement::stmt_ptr whileStatement();

    // expression parsing
    Expression::expr_ptr expression();
    Expression::expr_ptr assignment();
    Expression::expr_ptr orExpr();
    Expression::expr_ptr andExpr();
    Expression::expr_ptr equality();
    Expression::expr_ptr comparison();
    Expression::expr_ptr term();
    Expression::expr_ptr factor();
    Expression::expr_ptr unary();
    Expression::expr_ptr call();
    Expression::expr_ptr primary();

    Expression::expr_ptr finishCall(Expression::expr_ptr);

    // discard tokens until next statement (called when a sysntax error appears)
    void synchronize();

    // ---- helper functions ----

    // check if current token has any of the given types
    bool match(const std::vector<TokenType> &&);
    bool match(const TokenType &); // same but only for 1 TokenType

    // report error if next token isnt the expected one (and advance)
    Token consume(TokenType type, const std::string &&message);

    // returns true if the current token matches the given type
    bool check(TokenType) const;

    // consumes current token and returns it
    Token advance();

    inline Token peek() const
    {
        return _tokens.at(_current);
    }

    inline Token previous() const
    {
        return _tokens.at(_current - 1);
    }

    inline bool isAtEnd() const
    {
        return peek().type == TokenType::Eof;
    }

  private:
    const std::vector<Token> _tokens;
    int _current{0};
};
} // namespace lox

#endif
