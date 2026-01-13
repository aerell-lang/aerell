// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/token/token.hpp"

namespace aerell
{

Token::Token(File& file) : file(file), kind(TokenKind::EOFF), offset(0), size(0) {}

TokenKind Token::getKind() const { return this->kind; }

void Token::setKind(TokenKind kind) { this->kind = kind; }

std::size_t Token::getOffset() const { return this->offset; }
void Token::setOffset(std::size_t offset) { this->offset = offset; }

std::size_t Token::getSize() const { return this->size; }
void Token::setSize(std::size_t size) { this->size = size; }

} // namespace aerell