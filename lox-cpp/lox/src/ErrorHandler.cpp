#include <iostream>

#include "../include/ErrorHandler.h"
#include "../include/Lox.h"

void ErrorHandler::error(int line, const char *message)
{
    report(line, "", message);
}

void ErrorHandler::report(int line, const char *where, const char *message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    lox::Lox::hadError = true;
}
