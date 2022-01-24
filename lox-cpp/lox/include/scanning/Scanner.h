#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

namespace lox
{

class Scanner
{
  public:
    Scanner(const std::string &source) : _source(source){};

    using tokenlist_t = std::vector<lox::Token>;
    tokenlist_t scanTokens();

  private:
    std::string _source;
    tokenlist_t _tokens;

    int _start = 0;
    int _current = 0;
    int _line = 1;

    // private methods
    void scanToken();
    void addToken(const TokenType &type, const Token::literal_t &literal = {});
    void string();
    void number();
    void identifier();
    void comment();
    void multiline_comment();

    // private helper methods
    bool match(char expected);
    bool isAtEnd() const;
    char advance();
    char peek();
    char peekNext();

    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
};
} // namespace lox

#endif
