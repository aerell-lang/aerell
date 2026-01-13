// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/file.hpp"
#include <fstream>

namespace aerell
{

File::File(std::string_view path) : path{std::filesystem::weakly_canonical(path)}
{
    std::ifstream file{this->path, std::ios::binary | std::ios::ate};
    if(!file.is_open()) return;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    this->content.resize(size);
    file.read(this->content.data(), size);

    file.close();
}

const char* File::getData() const { return this->content.data(); }

} // namespace aerell