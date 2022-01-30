#include <iostream>

#include "../include/ErrorHandler.h"
#include "../include/Lox.h"
#include "../include/scanning/Token.h"

void lox::ErrorHandler::error(int line, const std::string & message)
{
    report(line, "", message);
}

void lox::ErrorHandler::error(const Token &token, const std::string & message)
{
    if (token.type == TokenType::Eof)
        report(token.line, " at end", message);
    else
    {
        std::string where = " at '" + token.lexeme + "'";
        report(token.line, where, message);
    }
}

void lox::ErrorHandler::report(int line, const std::string &where, const std::string &message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    lox::Lox::hadError = true;
}
