#include "../include/types/Callables.h"
#include "../include/evaluating/Interpreter.h"
#include "../include/types/Throwables.h"
#include <chrono>

lox::literal_t lox::ClockFunction::call(Interpreter &, const std::vector<literal_t> &) const
{
    using namespace std::chrono;

    const auto timepoint = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return timepoint / 1000.0;
}

lox::literal_t lox::LoxFunction::call(Interpreter &interpreter, const std::vector<literal_t> &args) const
{
    Environment::environment_ptr env = std::make_shared<Environment>(_closure);
    for (int i = 0; i < _declaration._params.size(); ++i)
    {
        const std::string varName = _declaration._params.at(i).lexeme;
        env->define(varName, args.at(i));
    }

    try
    {
        interpreter.executeBlock(_declaration._body, env);
    }
    catch (const Return &e)
    {
        return e.value();
    }

    return nullptr;
}

lox::literal_t lox::InputFunction::call(Interpreter &, const std::vector<literal_t> &) const
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

lox::literal_t lox::NumberFunction::call(Interpreter &interpreter, const std::vector<literal_t> &args) const
{
    const literal_t value = args.at(0);

    if (std::holds_alternative<std::string>(value))
    {
        try
        {
            return std::stod(std::get<std::string>(value));
        }
        catch (std::exception &)
        {
            throw LoxRuntimeError{"Couldn't convert to a number.", *_lineToken};
        }
    }

    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value) ? 1.0 : 0.0;

    throw LoxRuntimeError{"Only strings and bools are convertable to numbers.", *_lineToken};
}
