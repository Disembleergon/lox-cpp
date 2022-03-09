#include "../include/evaluating/Interpreter.h"
#include "../include/AST/Statements.h"
#include "../include/ErrorHandler.h"
#include "../include/types/TokenType.h"

// for throwing in a while loop (break statement) -> gets catched so the while
// loop breaks
class Break : public std::exception
{
    // EMPTY
};

// ---------------------------------

lox::Interpreter::Interpreter() : _environment{std::make_shared<Environment>()}
{
}

void lox::Interpreter::interpret(const Statement::stmt_vec &stmts)
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

std::string lox::Interpreter::toString(const literal_t &val)
{
    // because toString() converts the _resultingLiteral member
    _resultingLiteral = val;
    return toString();
}

// ----------- evaluate statements ------------

void lox::Interpreter::visitIfStmt(const IfStatement &stmt)
{
    literal_t evaluatedCondition = getLiteral(stmt._condition);

    if (isTruthy(evaluatedCondition))
        stmt._thenBranch->accept(*this); // execute then branch
    else if (stmt._elseBranch)
        stmt._elseBranch->accept(*this); // execute else branch
}

void lox::Interpreter::visitBlockStmt(const BlockStatement &stmt)
{
    executeBlock(stmt._statements, std::make_shared<Environment>(_environment));
}

void lox::Interpreter::visitExpressionStmt(const ExpressionStatement &stmt)
{
    stmt._expr->accept(*this);
}

void lox::Interpreter::visitVarStmt(const VarStatement &stmt)
{
    literal_t value;
    if (stmt._initializer)
        value = getLiteral(stmt._initializer);

    // save variable
    _environment->define(stmt._name.lexeme, value);
}

void lox::Interpreter::visitPrintStmt(const PrintStatement &stmt)
{
    // get literal in form of a string
    stmt._expr->accept(*this);
    const std::string strLiteral = toString();

    std::cout << strLiteral << "\n";
}

void lox::Interpreter::visitWhileStmt(const WhileStatement &stmt)
{
    try
    {
        while (isTruthy(getLiteral(stmt._condition)))
        {
            stmt._body->accept(*this);
        }
    }
    catch (const Break &)
    {
        // while statement is now cancelled
    }
}

void lox::Interpreter::visitBreakStmt(const BreakStatement &)
{
    throw Break{}; // gets catched in a while loop
}

// ----------- evaluate expressions ------------

void lox::Interpreter::visitAssignExpr(const AssignExpression &expr)
{
    literal_t value = getLiteral(expr._value); // evaluate expression
    _environment->assign(expr._name, value);

    _resultingLiteral = value; // just to be sure
}

void lox::Interpreter::visitBinaryExpr(const BinaryExpression &expr)
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

void lox::Interpreter::visitGroupingExpr(const GroupingExpression &expr)
{
    expr._expression->accept(*this);
}

void lox::Interpreter::visitLiteralExpr(const LiteralExpression &expr)
{
    _resultingLiteral = expr._value;
}

void lox::Interpreter::visitLogicalExpr(const LogicalExpression &expr)
{
    const literal_t left = getLiteral(expr._left);
    const bool leftIsTruthy = isTruthy(left);

    if (expr._operator.type == TokenType::OR) // or
    {
        if (leftIsTruthy)
        {
            _resultingLiteral = left;
            return;
        }
    }
    else if (!leftIsTruthy) // and
    {
        _resultingLiteral = left; // false (or falsyness, could also return nil etc.)
    }

    _resultingLiteral = getLiteral(expr._right);
}

void lox::Interpreter::visitUnaryExpr(const UnaryExpression &expr)
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

void lox::Interpreter::visitVarExpr(const VarExpression &expr)
{
    _resultingLiteral = _environment->get(expr._name);
}

// ---- private area -----

// for block statements
void lox::Interpreter::executeBlock(const Statement::stmt_vec &stmts, Environment::environment_ptr environment)
{
    Environment::environment_ptr outer = this->_environment;

    try
    {
        this->_environment = environment; // use newly created environment

        for (const Statement::stmt_ptr &stmt : stmts)
            stmt->accept(*this); // execute
    }
    catch (std::runtime_error &)
    {
        this->_environment = outer; // exit block, so going back to old environment
    }

    // do it again, in case no exception was thrown
    this->_environment = outer;
}

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

    if (holds_alternative<string>(left) || holds_alternative<string>(right))
    {
        _resultingLiteral = toString(left) + toString(right);
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
