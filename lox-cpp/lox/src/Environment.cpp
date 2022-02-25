#include "../include/evaluating/Environment.h"
#include "../include/scanning/Token.h"

// for LoxRuntimeError class
#include "../include/evaluating/Interpreter.h"

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

    throw LoxRuntimeError{"Undefined variable '" + name.lexeme + "'.", name};
}

lox::literal_t lox::Environment::get(const Token &name)
{
    if (_values.contains(name.lexeme))
        return _values.at(name.lexeme);

    throw LoxRuntimeError{"Undefined variable '" + name.lexeme + "'.", name};
}
