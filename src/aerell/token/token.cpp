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

Token::Token() : _source(nullptr), _type{TokenType::ILLEGAL}, _offset{0}, _size{0} {}

Token::Token(const Source* source, TokenType type, size_t offset, size_t size)
    : _source(source), _type{type}, _offset{offset}, _size{size}
{
}

Token::Token(Token&& other)
{
    this->_type = other._type;
    other._type = TokenType::ILLEGAL;
    this->_source = other._source;
    other._source = nullptr;
    this->_offset = other._offset;
    other._offset = 0;
    this->_size = other._size;
    other._size = 0;
}

Token& Token::operator=(const Token& other)
{
    if(this == &other) return *this;
    this->_type = other._type;
    this->_source = other._source;
    this->_offset = other._offset;
    this->_size = other._size;
    return *this;
}

TokenType Token::type() const { return this->_type; }

std::string_view Token::lexeme() const { return this->_source->content().substr(this->_offset, this->_size); }

void Token::print(std::string_view message) const { this->_source->print(this->_offset, this->_size, message); }

void Token::print(OStream& os) const { os << toString(this->type()) << ' ' << this->lexeme(); }

} // namespace aerell