#ifndef CALLABLES_H
#define CALLABLES_H

#include "../AST/Statements.h"
#include "../evaluating/Environment.h"
#include "LoxLiterals.h"
#include <vector>

namespace lox
{
class Interpreter;

class LoxCallable
{
  public:
    using callable_ptr = std::shared_ptr<LoxCallable>;

    virtual constexpr int arity() const = 0;
    virtual literal_t call(Interpreter &, const std::vector<literal_t> &) const = 0;

    virtual std::string toString() const
    {
        return "<callable>";
    }

    void setLineToken(const Token &t)
    {
        _lineToken = std::make_unique<Token>(t);
    }

  protected:
    std::unique_ptr<Token> _lineToken; // optional, can be used for throwing a LoxRuntimeError in call()
};

class LoxFunction final : public LoxCallable
{
  public:
    LoxFunction(const FunctionStatement &decl, const Environment::environment_ptr &closure)
        : _declaration{decl}, _closure{closure}
    {
    }

    constexpr int arity() const override
    {
        return _declaration._params.size();
    }

    literal_t call(Interpreter &, const std::vector<literal_t> &) const override;

    std::string toString() const override
    {
        return "<fn " + _declaration._name.lexeme + ">";
    }

  private:
    const FunctionStatement _declaration;
    const Environment::environment_ptr _closure;
};

// ------ native functions --------

class ClockFunction final : public LoxCallable
{
  public:
    constexpr int arity() const override
    {
        return 0;
    }

    literal_t call(Interpreter &, const std::vector<literal_t> &) const override;

    std::string toString() const override
    {
        return "<native fn>";
    }
};

class InputFunction final : public LoxCallable
{
  public:
    constexpr int arity() const override
    {
        return 0;
    }

    literal_t call(Interpreter &, const std::vector<literal_t> &) const override;

    std::string toString() const override
    {
        return "<native fn>";
    }
};

class NumberFunction final : public LoxCallable
{
  public:
    constexpr int arity() const override
    {
        return 1;
    }

    literal_t call(Interpreter &, const std::vector<literal_t> &) const override;

    std::string toString() const override
    {
        return "<native fn>";
    }
};

} // namespace lox

#endif
