#include "../include/AST/AstPrinter.h"

void lox::AstPrinter::visitBinaryExpr(const BinaryExpression &expr)
{
    parenthesize(expr._operator.lexeme, {expr._left.get(), expr._right.get()});
}

void lox::AstPrinter::visitGroupingExpr(const GroupingExpression &expr)
{
    parenthesize("group", {expr._expression.get()});
}

void lox::AstPrinter::visitLiteralExpr(const LiteralExpression &expr)
{
    const literal_t &lit = expr._value;
    using namespace std;

    // handle std::variant

    if (holds_alternative<string>(lit))
        _ss << get<string>(lit);
    else if (holds_alternative<bool>(lit))
        _ss << (get<bool>(lit) ? "true" : "false");
    else if (holds_alternative<double>(lit))
        _ss << get<double>(lit);
    else if (holds_alternative<nullptr_t>(lit))
        _ss << "nil";
    else
        cout << "[ASTPrinter] Couldn't parse literal\n";
}

void lox::AstPrinter::visitUnaryExpr(const UnaryExpression &expr)
{
    parenthesize(expr._operator.lexeme, {expr._right.get()});
}

void lox::AstPrinter::parenthesize(const std::string &name, const std::vector<Expression *> &expressions)
{
    _ss << "(" << name;
    for (const Expression *expr : expressions)
    {
        _ss << " ";
        expr->accept(*this);
    }

    _ss << ")";
}
