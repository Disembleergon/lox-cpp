#ifndef VISITOR_H
#define VISITOR_H

#include <any>
#include <memory>

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

    virtual std::any visitBinaryExpr(const Binary &expr) = 0;
    virtual std::any visitGroupingExpr(const Grouping &expr) = 0;
    virtual std::any visitLiteralExpr(const Literal &expr) = 0;
    virtual std::any visitUnaryExpr(const Unary &expr) = 0;
};

} // namespace lox

#endif
