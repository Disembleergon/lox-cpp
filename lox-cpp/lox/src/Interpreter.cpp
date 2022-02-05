#include "../include/evaluating/Interpreter.h"
#include "../include/AST/Expressions.h"

void lox::Interpreter::visitBinaryExpr(const Binary &expr)
{
}

void lox::Interpreter::visitGroupingExpr(const Grouping &expr)
{
}

void lox::Interpreter::visitLiteralExpr(const Literal &expr)
{
    _resultingLiteral = expr._value;
}

void lox::Interpreter::visitUnaryExpr(const Unary &expr)
{
}

// ---- private area -----

lox::literal_t lox::Interpreter::getLiteral(const Literal &lit)
{
    lit.accept(*this);
    return std::move(_resultingLiteral);
}
