#ifndef TOKEN_H
#define TOKEN_H

#include <iomanip>
#include <iostream>

#include "../types/TokenType.h"
#include "../types/LoxLiterals.h"

namespace lox
{

class Token
{
  public:
    const lox::TokenType type;
    const std::string lexeme;
    const literal_t literal;
    const int line;
};

} // namespace lox

#endif
