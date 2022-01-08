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
    using visitor_t = std::string; // TODO std::variant later? Currently I only need std::string
    virtual ~Visitor() = default;

    virtual visitor_t visitBinaryExpr(const Binary &expr) = 0;
    virtual visitor_t visitGroupingExpr(const Grouping &expr) = 0;
    virtual visitor_t visitLiteralExpr(const Literal &expr) = 0;
    virtual visitor_t visitUnaryExpr(const Unary &expr) = 0;
};

} // namespace lox

#endif
