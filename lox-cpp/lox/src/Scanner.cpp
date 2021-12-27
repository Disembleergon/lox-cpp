#include "../include/Scanner.h"

#include <stdexcept>

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
    m_tokens.push_back(Token{TokenType::Eof, "", {}, m_line});
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
        {
            // skip comment
            while (peek() != '\0' && !isAtEnd())
                advance();
        }
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

    // literal not found
    default:
        ErrorHandler::error(m_line, "Unexcpected character");
    }
}

void lox::Scanner::addToken(const TokenType &type, const Token::literal_t &literal)
{
    std::string text = m_source.substr(m_start, m_current);
    const lox::Token newToken{type, text, literal, m_line};
}

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

char lox::Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return m_source.at(m_current);
}
