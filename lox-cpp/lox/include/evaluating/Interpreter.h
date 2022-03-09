#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../AST/Expressions.h"
#include "../AST/Statements.h"
#include "../AST/Visitor.h"
#include "../types/LoxLiterals.h"
#include "Environment.h"

namespace lox
{

class LoxRuntimeError : public std::runtime_error
{
  private:
    const char *_msg;

  public:
    LoxRuntimeError(const std::string &msg, const Token &tok) : std::runtime_error{msg}, _msg{msg.c_str()}, token{tok}
    {
        // EMPTY
    }

    const char *what() const override
    {
        return _msg;
    }

    const Token token;
};

class Interpreter : public ExprVisitor, public StmtVisitor
{
  public:
    Interpreter();

    void interpret(const Statement::stmt_vec &stmts);
    std::string toString();
    std::string toString(const literal_t &val);

    // evaluating statements
    void visitIfStmt(const IfStatement &) override;
    void visitBlockStmt(const BlockStatement &) override;
    void visitExpressionStmt(const ExpressionStatement &) override;
    void visitVarStmt(const VarStatement &) override;
    void visitPrintStmt(const PrintStatement &) override;
    void visitWhileStmt(const WhileStatement &) override;
    void visitBreakStmt(const BreakStatement &) override;

    // evaluating expression
    void visitAssignExpr(const AssignExpression &expr) override;
    void visitBinaryExpr(const BinaryExpression &expr) override;
    void visitGroupingExpr(const GroupingExpression &expr) override;
    void visitLiteralExpr(const LiteralExpression &expr) override;
    void visitLogicalExpr(const LogicalExpression &expr) override;
    void visitUnaryExpr(const UnaryExpression &expr) override;
    void visitVarExpr(const VarExpression &expr) override;

  private:
    Environment::environment_ptr _environment; // for saving variables
    void executeBlock(const Statement::stmt_vec &stmts,
                      Environment::environment_ptr environment); // for block statements

    literal_t _resultingLiteral;
    literal_t getLiteral(const Expression::expr_ptr &expr);

    void evaluatePlus(const literal_t &left, const literal_t &right, const Token &op);
    bool isTruthy(const literal_t &lit);
    bool isEqual(const literal_t &a, const literal_t &b);

    // error handling / type checking
    void checkOperand(const Token &op, const literal_t &operand);
    void checkOperand(const Token &op, const literal_t &left, const literal_t &right);
};
} // namespace lox

#endif
