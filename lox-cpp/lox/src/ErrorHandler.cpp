#include <iostream>

#include "../include/ErrorHandler.h"
#include "../include/Lox.h"

void ErrorHandler::error(int line, const std::string &&message)
{
    report(line, "", message);
}

void ErrorHandler::report(int line, const std::string &where, const std::string &message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    lox::Lox::hadError = true;
}
