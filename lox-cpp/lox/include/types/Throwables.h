#ifndef THROWABLES_H
#define THROWABLES_H

#include "../scanning/Token.h"
#include <stdexcept>
#include <string>

namespace lox
{
class LoxRuntimeError : public std::runtime_error
{
  private:
    const std::string _msg;

  public:
    LoxRuntimeError(const std::string &msg, const Token &tok) : std::runtime_error{msg}, _msg{msg}, token{tok}
    {
        // EMPTY
    }

    const char *what() const noexcept override
    {
        return _msg.c_str();
    }

    const Token token;
};

// for throwing in a while loop (break statement) -> gets catched so the while
// loop breaks
class Break : public std::exception
{
    // EMPTY
};

class Return : public std::exception
{
  public:
    Return(const Token &keyword, const literal_t &value) : _value{value}, _keyword{keyword}
    {
    }

    literal_t value() const
    {
        return _value;
    }

    Token keyword() const
    {
        return _keyword;
    }

  private:
    const Token _keyword;
    const literal_t _value;
};

} // namespace lox

#endif
