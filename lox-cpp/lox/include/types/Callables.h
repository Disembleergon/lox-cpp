#ifndef CALLABLES_H
#define CALLABLES_H

#include "../types/LoxLiterals.h"
#include <vector>

namespace lox
{
class Interpreter;

class LoxCallable
{
  public:
    using callable_ptr = std::shared_ptr<LoxCallable>;

    virtual constexpr int arity() const = 0;
    virtual literal_t call(const Interpreter &, const std::vector<literal_t> &) const = 0;

    virtual std::string toString() const
    {
        return "<callable>";
    }
};

// ------ native functions --------

class ClockFunction final : public LoxCallable
{
  public:
    constexpr int arity() const override
    {
        return 0;
    }

    literal_t call(const Interpreter &, const std::vector<literal_t> &) const;

    std::string toString() const override
    {
        return "<native fn>";
    }
};

} // namespace lox

#endif
