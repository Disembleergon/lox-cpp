#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../types/LoxLiterals.h"
#include <unordered_map>

namespace lox
{
class Token;

// the place here all the variables (associations) are saved

class Environment
{
  public:
    void define(const std::string &name, const literal_t &value);
    void assign(const Token &name, const literal_t &value);
    literal_t get(const Token &name);

  private:
    std::unordered_map<std::string, literal_t> _values;
};

} // namespace lox

#endif
