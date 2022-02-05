#include "../include/evaluating/Interpreter.h"
#include "../include/types/TokenType.h"
#include <array>

void lox::Interpreter::visitBinaryExpr(const Binary &expr)
{
    literal_t left = getLiteral(expr._left);
    literal_t right = getLiteral(expr._right);

    using enum TokenType;
    using namespace std;

    switch (expr._operator.type)
    {
    case GREATER:
        _resultingLiteral = get<double>(left) > get<double>(right);
        break;
    case GREATER_EQUAL:
        _resultingLiteral = get<double>(left) >= get<double>(right);
        break;
    case LESS:
        _resultingLiteral = get<double>(left) < get<double>(right);
        break;
    case LESS_EQUAL:
        _resultingLiteral = get<double>(left) <= get<double>(right);
        break;
    case EQUAL_EQUAL:
        _resultingLiteral = isEqual(left, right);
        break;
    case BANG_EQUAL:
        _resultingLiteral = !isEqual(left, right);
        break;
    case MINUS:
        _resultingLiteral = get<double>(left) - get<double>(right);
        break;
    case PLUS:
        evaluatePlus(left, right);
        break;
    case SLASH:
        _resultingLiteral = get<double>(left) / get<double>(right);
        break;
    case STAR:
        _resultingLiteral = get<double>(left) * get<double>(right);
    }
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

void lox::Interpreter::evaluatePlus(const literal_t &left, const literal_t &right)
{
    using namespace std;

    if (holds_alternative<double>(left) && holds_alternative<double>(right))
    {
        _resultingLiteral = get<double>(left) + get<double>(right);
        return;
    }

    if (holds_alternative<string>(left) && holds_alternative<string>(right))
    {
        _resultingLiteral = get<string>(left) + get<string>(right);
        return;
    }
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

bool lox::Interpreter::isEqual(const literal_t &a, const literal_t &b)
{
    // not the same type
    if (a.index() != b.index())
        return false;

    using namespace std;

    if (holds_alternative<nullptr_t>(a))
        return true;
    if (holds_alternative<double>(a))
        return get<double>(a) == get<double>(b);
    if (holds_alternative<bool>(a))
        return get<bool>(a) == get<bool>(b);
    if (holds_alternative<string>(a))
        return get<string>(a) == get<string>(b);
}
