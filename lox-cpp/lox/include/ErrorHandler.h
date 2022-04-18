#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>

namespace lox
{
class Token;
class LoxRuntimeError;

class ErrorHandler
{
  public:
    static void error(int line, const std::string &message);
    static void error(const Token &token, const std::string &message);
    static void runtimeError(const LoxRuntimeError &e);

    static void report(int line, const std::string &where, const std::string &message);
};

} // namespace lox

#endif
