#include "../include/scanning/Scanner.h"
#include "../include/ErrorHandler.h"

lox::Scanner::tokenlist_t lox::Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        // start of new lexime
        m_start = m_current;
        scanToken();
    }

    // end of line
    m_tokens.emplace_back(Token{TokenType::Eof, "", {}, m_line});
    return m_tokens;
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
        ++m_line;
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
            ErrorHandler::error(m_line, "Unexcpected character");
    }
}

void lox::Scanner::addToken(const TokenType &type, const Token::literal_t &literal)
{
    std::string text = m_source.substr(m_start, m_current - m_start);
    const lox::Token newToken{type, text, literal, m_line};

    m_tokens.emplace_back(newToken);
}

void lox::Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            ++m_line;
        advance();
    }

    if (isAtEnd())
    {
        ErrorHandler::error(m_line, "Unterminated string.");
        return;
    }

    // The closing "
    advance();

    // trim surrounding quotes
    const auto value = m_source.substr(m_start + 1, m_current - m_start - 2);
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

    const auto str_value = m_source.substr(m_start, m_current - m_start);
    addToken(TokenType::NUMBER, std::stod(str_value));
}

void lox::Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    const auto lexeme = m_source.substr(m_start, m_current - m_start);
    TokenType type;

    try
    {
        type = Keywords.at(lexeme); // get TokenType from lexeme
    }
    catch (std::exception &e)
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
            ErrorHandler::error(m_line, "block-comment not closed.");
            return;
        }

        if (peek() == '\n')
            ++m_line;
        advance();
    }

    // skip '*/'
    advance();
    advance();
}

// check if next char == expected & advance
bool lox::Scanner::match(char expected)
{
    if (isAtEnd() || m_source.at(m_current) != expected)
        return false;

    ++m_current;
    return true;
}

bool lox::Scanner::isAtEnd() const
{
    return m_current >= m_source.length();
}

char lox::Scanner::advance()
{
    return m_source.at(m_current++);
}

// peek next character
char lox::Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return m_source.at(m_current);
}

// peek character after next character
char lox::Scanner::peekNext()
{
    if (m_current + 1 >= m_source.length())
        return '\0';

    return m_source.at(m_current + 1);
}

bool lox::Scanner::isAlpha(char c)
{
    return std::isalpha(c) || c == '_';
}

bool lox::Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || std::isdigit(c);
}
