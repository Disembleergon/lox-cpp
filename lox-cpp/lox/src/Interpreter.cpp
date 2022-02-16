#include "../include/evaluating/Interpreter.h"
#include "../include/AST/Statements.h"
#include "../include/ErrorHandler.h"
#include "../include/types/TokenType.h"

void lox::Interpreter::interpret(const std::vector<Statement::stmt_ptr> &stmts)
{
    try
    {
        for (const Statement::stmt_ptr &statement : stmts)
        {
            statement->accept(*this);
        }
    }
    catch (const LoxRuntimeError &e)
    {
        ErrorHandler::runtimeError(e);
    }
}

std::string lox::Interpreter::toString()
{
    using namespace std;

    if (holds_alternative<nullptr_t>(_resultingLiteral))
        return "nil";

    if (holds_alternative<double>(_resultingLiteral))
    {
        string strNum = to_string(get<double>(_resultingLiteral));

        // trim away thetrailing zeroes (.0000 etc.)
        if (strNum.find('.') != string::npos)
        {
            strNum = strNum.substr(0, strNum.find_last_not_of('0') + 1);

            // remove '.', if it's the last char
            if (strNum.find('.') == strNum.length() - 1)
                strNum = strNum.substr(0, strNum.length() - 1);
        }

        return strNum;
    }

    if (holds_alternative<string>(_resultingLiteral))
        return get<string>(_resultingLiteral);
    if (holds_alternative<bool>(_resultingLiteral))
        return get<bool>(_resultingLiteral) ? "true" : "false";
}

// ----------- evaluate statements ------------

void lox::Interpreter::visitExpressionStmt(const ExpressionStatement &stmt)
{
    stmt._expr->accept(*this);
}

void lox::Interpreter::visitPrintStmt(const PrintStatement &stmt)
{
    // get literal in form of a string
    stmt._expr->accept(*this);
    const std::string strLiteral = toString();

    std::cout << strLiteral << "\n";
}

// ----------- evaluate expressions ------------

void lox::Interpreter::visitBinaryExpr(const Binary &expr)
{
    literal_t left = getLiteral(expr._left);
    literal_t right = getLiteral(expr._right);

    using enum TokenType;
    using namespace std;

    switch (expr._operator.type)
    {
    case GREATER:
        checkOperand(expr._operator, left, right);
        _resultingLiteral = get<double>(left) > get<double>(right);
        break;
    case GREATER_EQUAL:
        checkOperand(expr._operator, left, right);
        _resultingLiteral = get<double>(left) >= get<double>(right);
        break;
    case LESS:
        checkOperand(expr._operator, left, right);
        _resultingLiteral = get<double>(left) < get<double>(right);
        break;
    case LESS_EQUAL:
        checkOperand(expr._operator, left, right);
        _resultingLiteral = get<double>(left) <= get<double>(right);
        break;
    case EQUAL_EQUAL:
        _resultingLiteral = isEqual(left, right);
        break;
    case BANG_EQUAL:
        _resultingLiteral = !isEqual(left, right);
        break;
    case MINUS:
        checkOperand(expr._operator, left, right);
        _resultingLiteral = get<double>(left) - get<double>(right);
        break;
    case PLUS:
        evaluatePlus(left, right, expr._operator);
        break;
    case SLASH:
        checkOperand(expr._operator, left, right);

        if (get<double>(right) == 0)
            throw LoxRuntimeError("Can't divide by 0.", expr._operator);

        _resultingLiteral = get<double>(left) / get<double>(right);
        break;
    case STAR:
        checkOperand(expr._operator, left, right);
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
        checkOperand(expr._operator, right);
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

void lox::Interpreter::evaluatePlus(const literal_t &left, const literal_t &right, const Token &op)
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

    throw LoxRuntimeError("Operands must be two numbers or strings.", op);
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

// ----- error handling / type checking -----

void lox::Interpreter::checkOperand(const Token &op, const literal_t &operand)
{
    if (std::holds_alternative<double>(operand))
        return;

    throw LoxRuntimeError("Operand must be a number.", op);
}

void lox::Interpreter::checkOperand(const Token &op, const literal_t &left, const literal_t &right)
{
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return;

    throw LoxRuntimeError("Operands must be numbers.", op);
}
