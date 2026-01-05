/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/token/token.h"

namespace aerell
{

Token::Token() : type{TokenType::ILLEGAL}, offset{0}, size{0} {}

Token::Token(TokenType type, size_t offset, size_t size) : type{type}, offset{offset}, size{size} {}

TokenType Token::getType() const { return this->type; }

size_t Token::getOffset() const { return this->offset; }

size_t Token::getSize() const { return this->size; }

void Token::print(OStream& os) const { os << "{ offset: " << this->offset << " size: " << this->size << " }"; }

} // namespace aerell