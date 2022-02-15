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
    virtual void accept(Visitor &) const = 0;
};

class ExpressionStatement final : public Statement
{
  public:
    ExpressionStatement(Expression::expr_ptr &expr) : _expr{std::move(expr)}
    {
    }

    Expression::expr_ptr _expr;

    void accept(Visitor &) const override{};
};

class PrintStatement final : public Statement
{
  public:
    PrintStatement(Expression::expr_ptr &expr) : _expr{std::move(expr)}
    {
    }

    Expression::expr_ptr _expr;

    void accept(Visitor &) const override{};
};

} // namespace lox

#endif