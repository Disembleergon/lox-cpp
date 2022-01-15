#include "../include/AST/AstPrinter.h"

void lox::AstPrinter::visitBinaryExpr(const Binary &expr)
{
    parenthesize(expr._operator.lexeme, {expr._left.get(), expr._right.get()});
}

void lox::AstPrinter::visitGroupingExpr(const Grouping &expr)
{
    parenthesize("group", {expr._expression.get()});
}

void lox::AstPrinter::visitLiteralExpr(const Literal &expr)
{
    // handle std::variant

    if (std::holds_alternative<std::string>(expr._value))
        m_ss << std::get<std::string>(expr._value);
    else if (std::holds_alternative<double>(expr._value))
        m_ss << std::get<double>(expr._value);
    else
        std::cout << "[ASTPrinter] Couldn't parse literal\n";
}

void lox::AstPrinter::visitUnaryExpr(const Unary &expr)
{
    parenthesize(expr._operator.lexeme, {expr._right.get()});
}

void lox::AstPrinter::parenthesize(const std::string &name, const std::vector<Expression *> &expressions)
{
    m_ss << "(" << name;
    for (const Expression *expr : expressions)
    {
        m_ss << " ";
        expr->accept(*this);
    }

    m_ss << ")";
}
