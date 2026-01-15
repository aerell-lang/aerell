// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <format>

#include "aerell/token/token.hpp"

namespace aerell
{

Token::Token(const File& file) : file{file}, kind{TokenKind::EOFF}, lexeme{0, 0} {}

TokenKind Token::getKind() const { return this->kind; }

void Token::setKind(TokenKind kind) { this->kind = kind; }

std::uint32_t Token::getOffset() const { return this->lexeme.offset; }
void Token::setOffset(std::uint32_t offset) { this->lexeme.offset = offset; }

std::uint32_t Token::getSize() const { return this->lexeme.size; }
void Token::setSize(std::uint32_t size) { this->lexeme.size = size; }

std::string Token::toStr() const
{
    return std::format(
        "[{}] {{offset: {}, size: {}, lexeme: {}}}", aerell::toStr(this->kind), this->getOffset(), this->getSize(),
        this->file.getLexemeText(this->lexeme));
}

} // namespace aerell