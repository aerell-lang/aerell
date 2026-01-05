/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <filesystem>
#include <memory>

#include "aerell/source/source_manager.h"

namespace aerell
{

int SourceManager::import(std::string_view path, bool makeSearchPath, OStream* errs)
{
    auto result = this->find(path);
    if(!result.has_value())
    {
        if(errs != nullptr) (*errs) << "Source not found";
        return 0;
    }

    const auto& path2 = result.value();

    auto [it, inserted] = this->files.emplace(path2);
    if(!inserted) return 2;

    if(makeSearchPath) this->searchPaths.emplace_back(path2.parent_path());

    auto newSource = std::make_unique<Source>(*it);
    if(!newSource->read(errs))
    {
        this->files.erase(it);
        return 0;
    }

    this->sources.push_back(std::move(newSource));
    return 1;
}

const Source& SourceManager::last() { return *this->sources.back(); }

std::optional<std::filesystem::path> SourceManager::find(std::string_view path)
{
    std::filesystem::path targetPath(path);
    targetPath.replace_extension(".arl");

    auto getFinalPath = [](const std::filesystem::path& p) -> std::optional<std::filesystem::path> {
        std::error_code ec;
        if(std::filesystem::is_regular_file(p, ec)) return std::filesystem::weakly_canonical(p);
        return std::nullopt;
    };

    if(auto res = getFinalPath(targetPath)) return res;

    std::filesystem::path _searchPath;
    for(const auto& searchPath : this->searchPaths)
    {
        _searchPath = searchPath;
        _searchPath /= targetPath;

        if(auto res = getFinalPath(_searchPath)) return res;
    }

    return std::nullopt;
}

} // namespace aerell