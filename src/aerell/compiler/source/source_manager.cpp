/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <algorithm>
#include <filesystem>
#include <memory>

#include "aerell/compiler/source/source_manager.h"

namespace aerell
{

int SourceManager::import(const char* value, llvm::raw_ostream& errs, bool makeAsPrefix)
{
    auto path = this->find(value);
    if(!path.has_value())
    {
        errs << "Source not found";
        return 0;
    }

    if(this->contain(path.value())) return 2;

    if(makeAsPrefix) this->importSourcePrefixs.push_back(path->parent_path().generic_string());

    Source source(std::move(path.value()));
    if(!source.read(errs)) return 0;
    this->sources.push_back(std::make_unique<Source>(std::move(source)));

    return 1;
}

bool SourceManager::contain(const std::filesystem::path& path)
{
    return std::any_of(
        this->sources.begin(), this->sources.end(), [&](const auto& source) { return source->getPath() == path; });
}

Source* SourceManager::getLastSource() { return this->sources.back().get(); }

bool SourceManager::hasSource() { return !this->sources.empty(); }

std::optional<std::filesystem::path> SourceManager::find(const char* value)
{
    auto path = std::filesystem::weakly_canonical(value).replace_extension("arl");
    if(!std::filesystem::exists(path) || std::filesystem::is_directory(path))
    {
        for(const auto& prefix : this->importSourcePrefixs)
        {
            path = std::filesystem::weakly_canonical(prefix + "/" + value).replace_extension("arl");
            if(!std::filesystem::exists(path) || std::filesystem::is_directory(path)) continue;
            return path;
        }
        return std::nullopt;
    }
    return path;
}

} // namespace aerell