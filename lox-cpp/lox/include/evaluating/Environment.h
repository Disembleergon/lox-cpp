#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../types/LoxLiterals.h"
#include <memory>
#include <unordered_map>

namespace lox
{
class Token;

// the place here all the variables (associations) are saved for one scope each

class Environment
{
  public:
    using environment_ptr = std::shared_ptr<Environment>;
    Environment(environment_ptr enclosing = nullptr);

    void define(const std::string &name, const literal_t &value);
    void assign(const Token &name, const literal_t &value);
    literal_t get(const Token &name);

  private:
    environment_ptr _enclosing; // [optional] holds the environment from the outer scope
    std::unordered_map<std::string, literal_t> _values;
};

} // namespace lox

#endif
