// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>

#include "aerell/file.hpp"

namespace aerell
{

std::optional<File> File::load(std::string_view path)
{
    // Prepare the result
    File result;
    result.setPath(path);

    // Open file
    std::ifstream file{result.getPath().generic_string(), std::ios::binary | std::ios::ate};
    if(!file.is_open()) return std::nullopt;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    result.content.resize(size);
    file.read(result.content.data(), size);

    file.close();

    // Build lineStartOffsets
    result.lineStartOffsets.push_back(0);
    for(std::uint32_t i = 0; i < static_cast<std::uint32_t>(result.content.length()); i++)
        if(result.content[i] == '\n') result.lineStartOffsets.push_back(i + 1);

    return result;
}

void File::setPath(std::string_view path) { this->path = std::filesystem::weakly_canonical(path); }

const std::filesystem::path& File::getPath() const { return this->path; }

const char* File::getData() const { return this->content.data(); }

std::uint32_t File::getLine(std::uint32_t offset) const
{
    assert(offset < this->content.length());
    if(offset >= this->content.length()) return 0;
    auto it = std::upper_bound(this->lineStartOffsets.begin(), lineStartOffsets.end(), offset);
    return std::distance(this->lineStartOffsets.begin(), it);
}

std::uint32_t File::getColumn(std::uint32_t line, std::uint32_t offset) const
{
    assert(line != 0 && line <= this->lineStartOffsets.size());
    assert(offset < this->content.length());
    if(line == 0 || line > this->lineStartOffsets.size() || offset >= this->content.length()) return 0;
    return offset - lineStartOffsets[line - 1] + 1;
}

File::Location File::toLocation(const Lexeme& lexeme) const
{
    std::uint32_t line = this->getLine(lexeme.offset);
    std::uint32_t column = this->getColumn(line, lexeme.offset);
    return {line, column};
}

std::uint32_t File::getOffsetFromLine(std::uint32_t line) const
{
    assert(line != 0 && line <= this->lineStartOffsets.size());
    if(line == 0 || line > this->lineStartOffsets.size()) return 0;
    return this->lineStartOffsets[line - 1];
}

std::string_view File::getLineText(std::uint32_t line) const
{
    if(line == 0 || line > lineStartOffsets.size()) return "";

    std::uint32_t start = this->getOffsetFromLine(line);
    std::uint32_t end;

    if(line < lineStartOffsets.size()) end = lineStartOffsets[line] - 1;
    else
        end = static_cast<std::uint32_t>(this->content.length());

    return std::string_view(this->content.data() + start, end - start);
}

} // namespace aerell