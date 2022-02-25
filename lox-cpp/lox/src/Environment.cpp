#include "../include/evaluating/Environment.h"
#include "../include/scanning/Token.h"

// for LoxRuntimeError class
#include "../include/evaluating/Interpreter.h"

void lox::Environment::define(const std::string &name, const literal_t &value)
{
    // update value or add a new one
    _values[name] = value;
}

lox::literal_t lox::Environment::get(const Token &name)
{
    if (_values.contains(name.lexeme))
        return _values.at(name.lexeme);

    throw LoxRuntimeError{"Undefined variable '" + name.lexeme + "'.", name};
}
