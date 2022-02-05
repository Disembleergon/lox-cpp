#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../AST/Visitor.h"
#include "../include/AST/Expressions.h"
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
    literal_t getLiteral(const Expression::expr_ptr &expr);

    void evaluatePlus(const literal_t &left, const literal_t &right);
    bool isTruthy(const literal_t &lit);
    bool isEqual(const literal_t &a, const literal_t &b);
};
} // namespace lox

#endif