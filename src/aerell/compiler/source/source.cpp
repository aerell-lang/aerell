/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <format>
#include <fstream>

#include "aerell/compiler/source/source.h"

namespace Aerell
{

Source::Source(std::filesystem::path&& path) : path(std::move(path)), pathStr(this->path.generic_string()) {}

bool Source::read(llvm::raw_ostream& errs)
{
    if(!this->content.empty()) this->content.clear();
    if(!this->lineStarts.empty()) this->lineStarts.clear();

    std::ifstream file(this->path, std::ios::binary);
    if(!file.is_open())
    {
        errs << "Could not open source";
        return false;
    }

    std::vector<unsigned char> fileBytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    this->content = {fileBytes.begin(), fileBytes.end()};

    lineStarts.push_back(0);
    for(size_t i = 0; i < this->content.size(); ++i)
        if(this->content[i] == '\n') lineStarts.push_back(i + 1);

    return true;
}

const std::string& Source::getContent() const { return this->content; }

const char* Source::getContentData() const { return this->content.data(); }

const std::string& Source::getPath() const { return this->pathStr; }

void Source::printErrorMessage(size_t offset, size_t size, const char* msg)
{
    // Find line and column from offset
    auto it = std::upper_bound(lineStarts.begin(), lineStarts.end(), offset);

    size_t line = (it - lineStarts.begin());
    size_t lineStart = lineStarts[line - 1];
    size_t column = offset - lineStart + 1;

    // Prepare print message
    auto path = std::format("{}:{}:{}", this->getPath(), line, column);
    auto prefix = std::format(" {}| ", line);
    size_t end = (line < lineStarts.size()) ? lineStarts[line] : content.size();
    auto view = std::string_view{content.data() + lineStart, end - lineStart};
    if(!view.empty() && view.back() == '\n') view.remove_suffix(1);

    // Print Message
    llvm::errs() << std::string(path.size(), '-') << '\n';
    llvm::errs() << path << "\n\n";
    llvm::errs() << prefix << view << "\n";
    llvm::errs() << std::string(prefix.size() + (column - 1), ' ') << std::string(size, '^') << '\n';
    llvm::errs() << ' ' << msg << '\n';
}

} // namespace Aerell