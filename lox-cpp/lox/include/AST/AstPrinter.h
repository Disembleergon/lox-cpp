#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "Expressions.h"
#include "Visitor.h"

#include <sstream>

namespace lox
{

class AstPrinter : public Visitor
{
  public:
    void visitBinaryExpr(const Binary &expr) override;
    void visitGroupingExpr(const Grouping &expr) override;
    void visitLiteralExpr(const Literal &expr) override;
    void visitUnaryExpr(const Unary &expr) override;

    std::string toString() const
    {
        return m_ss.str();
    }

  private:
    std::stringstream m_ss;
    void parenthesize(const std::string &name, const std::vector<Expression *> &expressions);
};

} // namespace lox

#endif
