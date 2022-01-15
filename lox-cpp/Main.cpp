#include "lox/include/AST/AstPrinter.h"
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

    /*using namespace lox;

    Expression::expr_ptr lit1 = std::make_unique<Literal>(Literal{{123.0}});
    Expression::expr_ptr lit2 = std::make_unique<Literal>(Literal{{45.67}});

    Expression::expr_ptr un_expr = std::make_unique<Unary>(Unary{Token{TokenType::MINUS, "-", {}, 1}, lit1});
    Token star{TokenType::STAR, "*", {}, 1};
    Expression::expr_ptr group = std::make_unique<Grouping>(Grouping{lit2});

    Binary expression{un_expr, star, group};

    AstPrinter printer;
    expression.accept(printer);
    std::cout << printer.toString() << std::endl;*/
}
