#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "Visitor.h"
#include "../scanning/Token.h"
#include "../types/LoxLiterals.h"

//#include <memory>

namespace lox
{
// abstract Expression class
class Expression
{
  public:
    Expression() = default;
    virtual ~Expression() = default;

    using expr_ptr = std::unique_ptr<Expression>;
    virtual void accept(ExprVisitor &) const = 0;
};

class Binary final : public Expression
{
  public:
    expr_ptr _left;
    Token _operator;
    expr_ptr _right;

    Binary(expr_ptr &left, const Token &op, expr_ptr &right)
        : _left{std::move(left)}, _operator{op}, _right{std::move(right)}
    {
    }

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitBinaryExpr(*this);
    }
};

class Grouping final : public Expression
{
  public:
    Grouping(expr_ptr &expr) : _expression{std::move(expr)}
    {
    }

    expr_ptr _expression;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitGroupingExpr(*this);
    }
};

class Literal final : public Expression
{
  public:
    Literal(const literal_t &val) : _value{val}
    {
    }

    literal_t _value;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitLiteralExpr(*this);
    }
};

class Unary final : public Expression
{
  public:
    Unary(const Token &op, expr_ptr &right) : _operator{op}, _right{std::move(right)}
    {
    }

    Token _operator;
    expr_ptr _right;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitUnaryExpr(*this);
    }
};

} // namespace lox

#endif
