#ifndef LOXLITERALS_H
#define LOXLITERALS_H

#include <string>
#include <variant>

namespace lox
{
// represents literals in Lox
using literal_t = std::variant<std::nullptr_t, std::string, double, bool>;

} // namespace lox

#endif
