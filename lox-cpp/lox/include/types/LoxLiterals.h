#ifndef LOXLITERALS_H
#define LOXLITERALS_H

#include <string>
#include <variant>
#include <memory>

namespace lox
{
class LoxCallable;
// represents literals in Lox
using literal_t = std::variant<std::nullptr_t, std::string, double, bool, std::shared_ptr<LoxCallable>>;

} // namespace lox

#endif
