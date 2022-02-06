#ifndef LOX_H
#define LOX_H

#include <iostream>

namespace lox
{
class Interpreter;

class Lox
{
  public:
    void runFile(const std::string &&filename);
    void runPrompt();
    void run(const std::string &sourceCode);

    // indicates, if any errors were found
    static bool hadError;
    static bool hadRuntimeError;

  private:
    static Interpreter _interpreter;
};

} // namespace lox

#endif
