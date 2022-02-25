#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "Expressions.h"
#include "Visitor.h"

#include <sstream>

namespace lox
{

class AstPrinter : public ExprVisitor
{
  public:
    void visitBinaryExpr(const BinaryExpression &expr) override;
    void visitGroupingExpr(const GroupingExpression &expr) override;
    void visitLiteralExpr(const LiteralExpression &expr) override;
    void visitUnaryExpr(const UnaryExpression &expr) override;

    std::string toString() const
    {
        return _ss.str();
    }

  private:
    std::stringstream _ss;
    void parenthesize(const std::string &name, const std::vector<Expression *> &expressions);
};

} // namespace lox

#endif
