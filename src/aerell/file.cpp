// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/file.hpp"
#include <fstream>

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
        if(result.content[i] == '\n') result.lineStartOffsets.push_back(i);

    return result;
}

void File::setPath(std::string_view path) { this->path = std::filesystem::weakly_canonical(path); }

const std::filesystem::path& File::getPath() const { return this->path; }

const char* File::getData() const { return this->content.data(); }

} // namespace aerell