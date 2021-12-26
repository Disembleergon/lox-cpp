#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

class Scanner
{
    using tokenlist_t = std::vector<lox::Token>;

  public:
    Scanner(const std::string &source) : m_source(source){};

    tokenlist_t scanTokens();

  private:
    std::string m_source;
    tokenlist_t m_tokens;

    int m_start = 0;
    int m_current = 0;
    int m_line = 1;

    // private methods
    void scanToken();
    void addToken(const lox::TokenType &type, const lox::Token::literal_t &literal = {});

    // private helper methods
    bool match(char expected);
    bool isAtEnd() const;
    char advance();
    char peek();
};

#endif
