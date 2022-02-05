#include "../include/evaluating/Interpreter.h"
#include "../include/types/TokenType.h"

void lox::Interpreter::visitBinaryExpr(const Binary &expr)
{
}

void lox::Interpreter::visitGroupingExpr(const Grouping &expr)
{
    expr._expression->accept(*this);
}

void lox::Interpreter::visitLiteralExpr(const Literal &expr)
{
    _resultingLiteral = expr._value;
}

void lox::Interpreter::visitUnaryExpr(const Unary &expr)
{
    literal_t right = getLiteral(expr._right);
    using enum TokenType;

    switch (expr._operator.type)
    {
    case BANG:
        _resultingLiteral = !isTruthy(right);
        break;
    case MINUS:
        _resultingLiteral = -std::get<double>(right);
    }

    _resultingLiteral = nullptr;
}

// ---- private area -----

lox::literal_t lox::Interpreter::getLiteral(const Expression::expr_ptr &expr)
{
    expr->accept(*this);
    return std::move(_resultingLiteral);
}

bool lox::Interpreter::isTruthy(const literal_t &lit)
{
    // false and nil are falsey, and everything else is truthy

    if (std::holds_alternative<std::nullptr_t>(lit))
        return false;
    if (std::holds_alternative<bool>(lit))
        return std::get<bool>(lit);

    return true;
}
