#ifndef VISITOR_H
#define VISITOR_H

namespace lox
{
class Binary;
class Grouping;
class Literal;
class Unary;

// abstract Visitor class
class Visitor
{
  public:
    virtual ~Visitor() = default;

    virtual void visitBinaryExpr(const Binary &expr) = 0;
    virtual void visitGroupingExpr(const Grouping &expr) = 0;
    virtual void visitLiteralExpr(const Literal &expr) = 0;
    virtual void visitUnaryExpr(const Unary &expr) = 0;
};

} // namespace lox

#endif
