#pragma once
#include <iostream>

namespace lox
{

class Lox
{
  public:
    void runFile(const char *filename);
    void runPrompt();
    void run(std::string &sourceCode);

  private:
    // error handling
    void error(int line, const char *message);
    void report(int line, const char *where, const char *message);

    // indicates, if any errors were found
    bool m_hadError = false;
};

} // namespace lox
