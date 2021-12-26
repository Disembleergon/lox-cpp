#ifndef LOX_H
#define LOX_H

#include <iostream>

namespace lox
{

class Lox
{
  public:
    void runFile(const char *filename);
    void runPrompt();
    void run(std::string &sourceCode);

    // indicates, if any errors were found
    static bool hadError;
};

} // namespace lox

#endif