#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "../scanning/Token.h"
#include "../types/LoxLiterals.h"
#include "Visitor.h"
#include <vector>

namespace lox
{
// abstract Expression class
class Expression
{
  public:
    Expression() = default;
    virtual ~Expression() = default;

    using expr_ptr = std::shared_ptr<Expression>;
    using expr_vec = std::vector<expr_ptr>;
    virtual void accept(ExprVisitor &) const = 0;
};

class AssignExpression final : public Expression
{
  public:
    AssignExpression(const Token &name, Expression::expr_ptr &value) : _name{name}, _value{std::move(value)}
    {
    }

    const Token _name;
    Expression::expr_ptr _value;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitAssignExpr(*this);
    }
};

class BinaryExpression final : public Expression
{
  public:
    expr_ptr _left;
    const Token _operator;
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

class CallExpression final : public Expression
{
  public:
    CallExpression(Expression::expr_ptr &callee, const Token &paren, Expression::expr_vec &args)
        : _callee{std::move(callee)}, _paren{paren}, _args{std::move(args)}
    {
    }

    Expression::expr_ptr _callee;
    const Token _paren;
    const Expression::expr_vec _args;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitCallExpr(*this);
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

    const literal_t _value;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitLiteralExpr(*this);
    }
};

class LogicalExpression final : public Expression
{
  public:
    LogicalExpression(expr_ptr &left, const Token &op, expr_ptr &right)
        : _left{std::move(left)}, _operator{op}, _right{std::move(right)}
    {
    }

    expr_ptr _left;
    const Token _operator;
    expr_ptr _right;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitLogicalExpr(*this);
    }
};

class UnaryExpression final : public Expression
{
  public:
    UnaryExpression(const Token &op, expr_ptr &right) : _operator{op}, _right{std::move(right)}
    {
    }

    const Token _operator;
    expr_ptr _right;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitUnaryExpr(*this);
    }
};

class VarExpression final : public Expression
{
  public:
    VarExpression(const Token &name) : _name{name}
    {
    }

    const Token _name;

    void accept(ExprVisitor &visitor) const override
    {
        visitor.visitVarExpr(*this);
    }
};

} // namespace lox

#endif
