#include "../include/Lox.h"
#include <fstream>
#include <string>

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
            sourceCode += line;

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
    if (m_hadError)
        std::exit(EXIT_FAILURE);
}

// execute lox commands in the cmd
void lox::Lox::runPrompt()
{
    std::string enteredSource;
    while (std::cout << "> " && std::getline(std::cin, enteredSource))
    {
        run(enteredSource);
        m_hadError = false;
    }
}

void lox::Lox::run(std::string &sourceCode)
{
}

// ---- error handling ----

void lox::Lox::error(int line, const char *message)
{
    report(line, "", message);
}

void lox::Lox::report(int line, const char *where, const char *message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    m_hadError = true;
}
