#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "Expressions.h"
#include <memory>

namespace lox
{
// abstract Statement class
class Statement
{
  public:
    Statement() = default;
    virtual ~Statement() = default;

    using stmt_ptr = std::unique_ptr<Statement>;
    using stmt_vec = std::vector<stmt_ptr>;

    virtual void accept(StmtVisitor &) const = 0;
};

class IfStatement final : public Statement
{
  public:
    IfStatement(Expression::expr_ptr &condition, stmt_ptr &thenB, stmt_ptr &elseB)
        : _condition{std::move(condition)}, _thenBranch{std::move(thenB)}, _elseBranch{std::move(elseB)}
    {
    }

    Expression::expr_ptr _condition;
    stmt_ptr _thenBranch;
    stmt_ptr _elseBranch;

    void accept(StmtVisitor &visitor) const override
    {
        visitor.visitIfStmt(*this);
    }
};

class BlockStatement final : public Statement
{
  public:
    BlockStatement(stmt_vec &statements) : _statements{std::move(statements)}
    {
    }

    const stmt_vec _statements;

    void accept(StmtVisitor &visitor) const override
    {
        visitor.visitBlockStmt(*this);
    }
};

class ExpressionStatement final : public Statement
{
  public:
    ExpressionStatement(Expression::expr_ptr &expr) : _expr{std::move(expr)}
    {
    }

    Expression::expr_ptr _expr;

    void accept(StmtVisitor &visitor) const override
    {
        visitor.visitExpressionStmt(*this);
    };
};

class PrintStatement final : public Statement
{
  public:
    PrintStatement(Expression::expr_ptr &expr) : _expr{std::move(expr)}
    {
    }

    Expression::expr_ptr _expr;

    void accept(StmtVisitor &visitor) const override
    {
        visitor.visitPrintStmt(*this);
    };
};

class VarStatement final : public Statement
{
  public:
    VarStatement(const Token &name, Expression::expr_ptr &initializer)
        : _name{name}, _initializer{std::move(initializer)}
    {
    }

    const Token _name;
    Expression::expr_ptr _initializer;

    void accept(StmtVisitor &visitor) const override
    {
        visitor.visitVarStmt(*this);
    }
};

class WhileStatement final : public Statement
{
  public:
    WhileStatement(Expression::expr_ptr &condition, Statement::stmt_ptr &body)
        : _condition{std::move(condition)}, _body{std::move(body)}
    {
    }

    Expression::expr_ptr _condition;
    Statement::stmt_ptr _body;

    void accept(StmtVisitor &visitor) const override
    {
        // TODO
    }
};

} // namespace lox

#endif
