#include "../include/Lox.h"
#include "../include/Scanner.h"

#include <fstream>
#include <string>

// init hadError variable
bool lox::Lox::hadError = false;

// run .lox file
void lox::Lox::runFile(const char *filename)
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

    for (const auto &token : tokens)
    {
        std::cout << token << "\n";
    }
}
