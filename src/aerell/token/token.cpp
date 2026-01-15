// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <format>

#include "aerell/token/token.hpp"

namespace aerell
{

Token::Token(const File& file) : file{file}, kind{TokenKind::EOFF}, lexeme{0, 0} {}

TokenKind Token::getKind() const { return this->kind; }

void Token::setKind(TokenKind kind) { this->kind = kind; }

void Token::setOffset(std::uint32_t offset) { this->lexeme.offset = offset; }

void Token::setSize(std::uint32_t size) { this->lexeme.size = size; }

std::string Token::toStr() const
{
    return std::format(
        "[{}] {{ offset: {}, size: {} }} {}", aerell::toStr(this->kind), this->getLexeme().offset,
        this->getLexeme().size, this->file.getLexemeText(this->getLexeme()));
}

const Lexeme& Token::getLexeme() const { return this->lexeme; }

} // namespace aerell