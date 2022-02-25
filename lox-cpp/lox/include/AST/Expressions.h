#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "../scanning/Token.h"
#include "../types/LoxLiterals.h"
#include "Visitor.h"

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

class BinaryExpression final : public Expression
{
  public:
    expr_ptr _left;
    Token _operator;
    expr_ptr _right;

    BinaryExpression(expr_ptr &left, const Token &op, expr_ptr &right)
        : _left{std::move(left)}, _operator{op}, _right{std::move(right)}
    {
    }

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitBinaryExpr(*this);
    }
};

class GroupingExpression final : public Expression
{
  public:
    GroupingExpression(expr_ptr &expr) : _expression{std::move(expr)}
    {
    }

    expr_ptr _expression;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitGroupingExpr(*this);
    }
};

class LiteralExpression final : public Expression
{
  public:
    LiteralExpression(const literal_t &val) : _value{val}
    {
    }

    literal_t _value;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitLiteralExpr(*this);
    }
};

class UnaryExpression final : public Expression
{
  public:
    UnaryExpression(const Token &op, expr_ptr &right) : _operator{op}, _right{std::move(right)}
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
