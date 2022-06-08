#include "../include/scanning/Scanner.h"
#include "../include/ErrorHandler.h"
#include "../include/types/LoxLiterals.h"

lox::Scanner::tokenlist_t lox::Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        // start of new lexime
        _start = _current;
        scanToken();
    }

    // end of line
    _tokens.emplace_back(Token{TokenType::Eof, "", {}, _line});
    return _tokens;
}

// scan next char and identify it
void lox::Scanner::scanToken()
{
    using enum TokenType;
    char c = advance(); // next char

    switch (c)
    {
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case ';':
        addToken(SEMICOLON);
        break;
    case '*':
        addToken(STAR);
        break;

    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '/':
        if (match('/'))
            comment();
        else if (match('*'))
            multiline_comment();
        else
            addToken(SLASH);
        break;

    // ignore whitespace
    case ' ':
    case '\r':
    case '\t':
        break;

    // next line
    case '\n':
        ++_line;
        break;

    // string literal
    case '"':
        string();
        break;

    // literal not found
    default:
        if (std::isdigit(c))
            number();
        else if (isAlpha(c))
            identifier();
        else
            ErrorHandler::error(_line, "Unexcpected character");
    }
}

void lox::Scanner::addToken(const TokenType &type, const literal_t &literal)
{
    std::string text = _source.substr(_start, _current - _start);
    const lox::Token newToken{type, text, literal, _line};

    _tokens.push_back(newToken);
}

void lox::Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            ++_line;
        advance();
    }

    if (isAtEnd())
    {
        ErrorHandler::error(_line, "Unterminated string.");
        return;
    }

    // The closing "
    advance();

    // trim surrounding quotes
    const auto value = _source.substr(_start + 1, _current - _start - 2);
    addToken(TokenType::STRING, value);
}

void lox::Scanner::number()
{
    while (std::isdigit(peek()))
        advance();

    // optional fractional part
    if (peek() == '.' && std::isdigit(peekNext()))
    {
        // consume the "."
        advance();

        while (std::isdigit(peek()))
            advance();
    }

    const auto str_value = _source.substr(_start, _current - _start);
    addToken(TokenType::NUMBER, std::stod(str_value));
}

void lox::Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    const auto lexeme = _source.substr(_start, _current - _start);
    TokenType type;

    try
    {
        type = Keywords.at(lexeme); // get TokenType from lexeme
    }
    catch (std::out_of_range &)
    {
        type = TokenType::IDENTIFIER; // not a keyword, it's an identifier!
    }

    addToken(type);
}

void lox::Scanner::comment()
{
    // skip comment
    while (peek() != '\n' && !isAtEnd())
        advance();
}

void lox::Scanner::multiline_comment()
{
    while (peek() != '*' || peekNext() != '/')
    {
        if (isAtEnd())
        {
            ErrorHandler::error(_line, "block-comment not closed.");
            return;
        }

        if (peek() == '\n')
            ++_line;
        advance();
    }

    // skip '*/'
    advance();
    advance();
}

// check if next char == expected & advance
bool lox::Scanner::match(char expected)
{
    if (isAtEnd() || _source.at(_current) != expected)
        return false;

    ++_current;
    return true;
}

bool lox::Scanner::isAtEnd() const
{
    return _current >= _source.length();
}

char lox::Scanner::advance()
{
    return _source.at(_current++);
}

// peek next character
char lox::Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return _source.at(_current);
}

// peek character after next character
char lox::Scanner::peekNext()
{
    if (_current + 1 >= _source.length())
        return '\0';

    return _source.at(_current + 1);
}

bool lox::Scanner::isAlpha(char c)
{
    return std::isalpha(c) || c == '_';
}

bool lox::Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || std::isdigit(c);
}
