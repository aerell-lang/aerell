/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_READER_CHAR_READER_H
#define AERELL_READER_CHAR_READER_H

#include <string_view>

namespace aerell
{

class CharReader
{
  public:
    void set(std::string_view data);

    // Reset state
    void reset();

    // Getter size
    size_t size() const;

    // Position
    size_t tell(size_t offset) const;
    size_t tell() const;
    void seek(size_t pos);
    bool canAdvance(size_t offset, size_t size) const;
    bool canAdvance(size_t size) const;
    bool canAdvance() const;
    void advance(size_t offset);
    void advance();

    // Char
    char peek(size_t offset);
    char peek();

    // String
    std::string_view peek(size_t offset, size_t size);

    // Comparation
    bool match(size_t offset, char c);
    bool match(char c);
    bool match(size_t offset, std::string_view str);
    bool match(std::string_view str);

  protected:
    size_t _pos{0};
    size_t _size{0};
    std::string_view _data;
};

} // namespace aerell

#endif