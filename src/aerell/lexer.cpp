// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/lexer.hpp"

namespace aerell
{

Lexer::Lexer(const File& file) : file(file), data(file.getData()) {}

const File& Lexer::getFile() const { return this->file; }

#define CURRENT_CHAR *this->data
#define NL '\n'
#define IS_EOF(x) (x == '\0')
#define IS_WS(x) (x == ' ' || x == '\t' || x == '\r' || x == NL)
#define IS_DIGIT(x) (x >= '0' && x <= '9')
#define IS_CHR(x, y) (x == y)
#define NEXT_CHAR this->data++
#define SET_OFFSET_AUTO this->token.setOffset(this->data - this->file.getData())
#define SET_KIND(x) this->token.setKind(TokenKind::x)
#define SET_SIZE(x) this->token.setSize(x)
#define SET_SIZE_AUTO SET_SIZE(this->data - (this->file.getData() + this->token.getOffset()))

const Token& Lexer::getToken() const { return this->token; }

void Lexer::forwardToken()
{
    // Skip
    while(!IS_EOF(CURRENT_CHAR))
    {
        // WS
        while(IS_WS(CURRENT_CHAR))
        {
            NEXT_CHAR;
            continue;
        }

        // COMMENT
        if(IS_CHR(CURRENT_CHAR, '#'))
        {
            NEXT_CHAR;
            while(!IS_EOF(CURRENT_CHAR) && !IS_CHR(CURRENT_CHAR, NL)) NEXT_CHAR;
            continue;
        }

        // Stop skip section
        break;
    }

    // EOF
    if(IS_EOF(CURRENT_CHAR))
    {
        SET_KIND(EOFF);
        SET_OFFSET_AUTO;
        SET_SIZE(0);
        return;
    }

    // INTL
    if(IS_DIGIT(CURRENT_CHAR))
    {
        SET_KIND(INTL);
        SET_OFFSET_AUTO;
        NEXT_CHAR;
        while(IS_DIGIT(CURRENT_CHAR)) NEXT_CHAR;
        SET_SIZE_AUTO;
        return;
    }

    // ILLEGAL
    SET_KIND(ILLEGAL);
    SET_OFFSET_AUTO;
    SET_SIZE(1);
    NEXT_CHAR;
}

} // namespace aerell