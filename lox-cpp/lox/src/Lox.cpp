#include "../include/Lox.h"
#include "../include/AST/AstPrinter.h"
#include "../include/parsing/Parser.h"
#include "../include/scanning/Scanner.h"

#include <fstream>
#include <string>

// init hadError variable
bool lox::Lox::hadError = false;

// run .lox file
void lox::Lox::runFile(const std::string &&filename)
{
    // ---- read in file to sourceCode string ----
    std::ifstream fileStream(filename);
    std::string sourceCode;

    if (fileStream)
    {
        std::string line;
        while (std::getline(fileStream, line))
            sourceCode += (line + '\n');

        fileStream.close();
    }
    else
    {
        std::cerr << "Failed to open file!" << std::endl;
        fileStream.close();
        std::exit(EXIT_FAILURE);
    }

    run(sourceCode);

    // indicate error with exit code
    if (hadError)
        std::exit(EXIT_FAILURE);
}

// execute lox commands in the cmd
void lox::Lox::runPrompt()
{
    std::string enteredSource;
    while (std::cout << "> " && std::getline(std::cin, enteredSource))
    {
        run(enteredSource);
        hadError = false;
    }
}

void lox::Lox::run(const std::string &sourceCode)
{
    Scanner scanner{sourceCode};
    Scanner::tokenlist_t tokens = scanner.scanTokens();

    Parser parser{tokens};
    Expression::expr_ptr expr = parser.parse();

    if (hadError)
        return;

    // print expression
    AstPrinter printer;
    expr->accept(printer);

    std::cout << printer.toString() << std::endl;
}
