#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "../scanning/Token.h"
#include "Visitor.h"

#include <memory>

namespace lox
{
// abstract Expression class
class Expression
{
  public:
    using expr_ptr = std::unique_ptr<Expression>;

    virtual std::any accept(Visitor &) = 0;

    // delete copy constructor & copy assignment operator
    Expression(const Expression &) = delete;
    Expression &operator=(const Expression &) = delete;
};

class Binary final : public Expression
{
  public:
    expr_ptr _left;
    Token _operator;
    expr_ptr _right;

    std::any accept(Visitor &visitor) override
    {
        return visitor.visitBinaryExpr(*this);
    }
};

class Grouping final : public Expression
{
  public:
    expr_ptr _expression;

    std::any accept(Visitor &visitor) override
    {
        return visitor.visitGroupingExpr(*this);
    }
};

class Literal final : public Expression
{
  public:
    Token::literal_t _value;

    std::any accept(Visitor &visitor) override
    {
        return visitor.visitLiteralExpr(*this);
    }
};

class Unary final : public Expression
{
  public:
    Token _operator;
    expr_ptr _right;

    std::any accept(Visitor &visitor) override
    {
        return visitor.visitUnaryExpr(*this);
    }
};

} // namespace lox

#endif
