/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <format>
#include <fstream>

#include "aerell/source/source.h"

namespace aerell
{

Source::Source(const std::filesystem::path& path) : _path{path} {}

bool Source::read(OStream* errs)
{
    // Open file
    std::ifstream file(this->_path, std::ios::binary);
    if(!file.is_open())
    {
        if(errs != nullptr) (*errs) << "Could not open source file";
        return false;
    }

    // Read file
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    this->_content.resize(size);
    if(size > 0) file.rdbuf()->sgetn(reinterpret_cast<char*>(this->_content.data()), size);

    // Build line start offsets
    this->_lineStartOffsets.reserve(this->_content.size() / 40);

    this->_lineStartOffsets.push_back(0);

    size_t pos = this->_content.find('\n');
    while(pos != std::string::npos)
    {
        this->_lineStartOffsets.push_back(pos + 1);
        pos = this->_content.find('\n', pos + 1);
    }

    return true;
}

const std::filesystem::path& Source::path() const { return this->_path; }

std::string_view Source::content() const { return this->_content; }

std::string_view Source::content(const Token& token) const
{
    return this->content().substr(token.getOffset(), token.getSize());
}

void Source::print(const Token& token, std::string_view message) const
{
    // Find line and column from offset
    auto it = std::upper_bound(this->_lineStartOffsets.begin(), this->_lineStartOffsets.end(), token.getOffset());

    size_t line = (it - this->_lineStartOffsets.begin());
    size_t lineStart = this->_lineStartOffsets[line - 1];
    size_t column = token.getOffset() - lineStart + 1;

    // Prepare print message
    auto path = std::format("{}:{}:{}", this->path().generic_string(), line, column);
    auto prefix = std::format(" {}| ", line);
    size_t end = (line < this->_lineStartOffsets.size()) ? this->_lineStartOffsets[line] : this->content().size();
    auto view = std::string_view{this->content().data() + lineStart, end - lineStart};
    if(!view.empty() && view.back() == '\n') view.remove_suffix(1);

    // Print Message
    errs() << std::string(path.size(), '-') << '\n';
    errs() << path << "\n\n";
    errs() << prefix << view << "\n";
    errs() << std::string(prefix.size() + (column - 1), ' ') << std::string(token.getSize(), '^') << '\n';
    errs() << ' ' << message << '\n';
}

} // namespace aerell