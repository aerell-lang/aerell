/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <string>

#include "aerell/reader/char_reader.h"

namespace aerell
{

void CharReader::set(std::string_view data)
{
    this->reset();
    this->_data = data;
    this->_size = this->_data.size();
}

// Reset state
void CharReader::reset()
{
    if(this->_pos != 0) this->_pos = 0;
    if(this->_size != 0) this->_size = 0;
}

// Getter size
size_t CharReader::size() const { return this->_size; }

// Position
size_t CharReader::tell(size_t offset) const { return this->_pos + offset; }

size_t CharReader::tell() const { return this->tell(0); }

void CharReader::seek(size_t pos) { this->_pos = pos; }

bool CharReader::canAdvance(size_t offset, size_t size) const { return (this->tell(offset) + size) <= this->size(); }

bool CharReader::canAdvance(size_t size) const { return this->canAdvance(0, size); }

bool CharReader::canAdvance() const { return this->canAdvance(1); }

void CharReader::advance(size_t offset) { this->seek(this->tell(offset)); }

void CharReader::advance() { this->advance(1); }

// Char
char CharReader::peek(size_t offset)
{
    if(!this->canAdvance(offset, 1))
        throw std::overflow_error(
            "CharReader Error: Read operation (offset: " + std::to_string(offset) + ") exceeds buffer limits.");

    return this->_data[this->tell(offset)];
}

char CharReader::peek() { return this->peek(0); }

// String
std::string_view CharReader::peek(size_t offset, size_t size)
{
    if(!this->canAdvance(offset, size))
        throw std::overflow_error(
            "CharReader Error: Read operation (offset: " + std::to_string(offset) + ", size: " + std::to_string(size) +
            ") exceeds buffer limits.");

    return {this->_data.data() + this->tell(offset), size};
}

// Comparation
bool CharReader::match(size_t offset, char c) { return this->peek(offset) == c; }

bool CharReader::match(char c) { return this->match(0, c); }

bool CharReader::match(size_t offset, std::string_view str) { return this->peek(offset, str.size()) == str; }

bool CharReader::match(std::string_view str) { return this->match(0, str); }

} // namespace aerell