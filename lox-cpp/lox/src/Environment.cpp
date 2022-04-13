#include "../include/evaluating/Environment.h"
#include "../include/scanning/Token.h"
#include "../include/types/Throwables.h"

lox::Environment::Environment(environment_ptr enclosing) : _enclosing{std::move(enclosing)}
{
}

void lox::Environment::define(const std::string &name, const literal_t &value)
{
    // update value or add a new one
    _values[name] = value;
}

void lox::Environment::assign(const Token &name, const literal_t &value)
{
    if (_values.contains(name.lexeme))
    {
        _values[name.lexeme] = value;
        return;
    }

    if (_enclosing)
    {
        _enclosing->assign(name, value);
        return;
    }

    throw LoxRuntimeError{"Undefined variable '" + name.lexeme + "'.", name};
}

lox::literal_t lox::Environment::get(const Token &name)
{
    if (_values.contains(name.lexeme))
        return _values.at(name.lexeme);

    if (_enclosing)
        return _enclosing->get(name);

    throw LoxRuntimeError{"Undefined variable '" + name.lexeme + "'.", name};
}
