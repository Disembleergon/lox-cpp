#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../AST/Visitor.h"
#include "../types/LoxLiterals.h"

namespace lox
{
class Interpreter : public Visitor
{
  public:
    void visitBinaryExpr(const Binary &expr) override;
    void visitGroupingExpr(const Grouping &expr) override;
    void visitLiteralExpr(const Literal &expr) override;
    void visitUnaryExpr(const Unary &expr) override;

  private:
    literal_t _resultingLiteral;
    literal_t getLiteral(const Literal &lit);
};
} // namespace lox

#endif
