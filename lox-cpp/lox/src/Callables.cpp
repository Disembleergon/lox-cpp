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
