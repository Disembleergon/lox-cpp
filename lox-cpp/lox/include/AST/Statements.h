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
        // TODO
    }
};

} // namespace lox

#endif
