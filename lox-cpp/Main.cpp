#include "lox/include/Lox.h"

int main(int argc, char *argv[])
{
    if (argc > 2) // too many arguments
    {
        std::cout << "Usage: lox-cpp [script]" << std::endl;
        return EXIT_FAILURE;
    }

    lox::Lox lox;

    if (argc == 2)
        lox.runFile(argv[1]);
    else
        lox.runPrompt();
}
